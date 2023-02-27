/*
 * This file is part of Rapid Reservoir Modelling Software.
 *   | https://rapidreservoir.org/
 *   | https://bitbucket.org/rapidreservoirmodelling/rrm/
 *
 * Copyright (c) 2022
 *   | Dmytro Petrovskyy, PhD
 *   | dmytro.petrovsky@gmail.com
 *   | https://www.linkedin.com/in/dmytro-petrovskyy/
 *
 * RRM is free software: you can redistribute it and/or modify              
 * it under the terms of the GNU General Public License as published by     
 * the Free Software Foundation, either version 3 of the License, or        
 * (at your option) any later version.                                      
 *                                                                         
 * RRM is distributed in the hope that it will be useful,                   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            
 * GNU General Public License for more details.                             
 *                                                                         
 * You should have received a copy of the GNU General Public License        
 * along with RRM. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once
#include <QMetaObject>

#include <list>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>


namespace rrm::fd
{
  class AbstractTask
  {
  public:
    virtual ~AbstractTask() = default;

    virtual void Execute() = 0; // Concurrent to the UI thread
    virtual void Commit() = 0; // Within the UI thread
  };
 

  class TaskScheduler
  {
    class TaskDispatcher
    {
      std::thread back_thread_;

      TaskScheduler& queue_;

    public:
      bool end_thread_ = false;

      TaskDispatcher(TaskScheduler& queue): queue_(queue) {
        back_thread_ = std::thread([this]() {
          while (true) {
            std::unique_ptr<AbstractTask> task;
            std::mutex mutex;
            std::unique_lock unique_lock{mutex};
            
            queue_.Condition().wait(unique_lock, [this, &task] {
              task = queue_.Pop();
              return end_thread_ || task.get();
            });


            if (end_thread_)
              break;

            try {
              task->Execute();
              
              QMetaObject::invokeMethod(
                queue_.UIObject(), [t = std::move(task)]() {
                t->Commit();
              });
            }
            catch (...) {}
          }
        });
      }

      ~TaskDispatcher() {
        // {
        //   end_thread_ = true;
        // }

        // queue_.Condition().notify_all(); // TODO: notifies the thread of this object?

        // cond_var_.notify_one();

        if (back_thread_.joinable())
          back_thread_.join();
      }



    };

    
    std::mutex mutex_;

    std::queue<std::unique_ptr<AbstractTask>> tasks_;

    std::condition_variable cond_var_;

    QObject* ui_object_;

    std::list<TaskDispatcher> dispatchers_;
    
  public:
    ~TaskScheduler() {
      for (auto& disp : dispatchers_)
        disp.end_thread_ = true;

      cond_var_.notify_all();
    }

    explicit TaskScheduler(QObject* qobject, int dispatcher_count = 3)
      : ui_object_(qobject) {

      for (auto i = 0; i < dispatcher_count; ++i)
        dispatchers_.emplace_back(*this);
    }

    void Push(std::unique_ptr<AbstractTask>&& task) {
      {
        std::scoped_lock lock{mutex_};
        tasks_.push(std::move(task));
      }

      cond_var_.notify_one();
    }

    std::unique_ptr<AbstractTask> Pop() {
      std::scoped_lock lock{mutex_};

      if (tasks_.empty())
        return nullptr;

      auto front = std::move(tasks_.front());
      tasks_.pop();

      return front;
    }

    std::condition_variable& Condition() {
      return cond_var_;
    }

    QObject* UIObject() {
      return ui_object_;
    }
  };
}
