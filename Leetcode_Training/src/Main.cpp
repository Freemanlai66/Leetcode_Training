#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <unordered_set>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
	// 函数声明...xx
#ifdef __cplusplus
}
#endif

queue<int> taskQueue;
mutex mtx;
condition_variable cv;
atomic<bool> shutdown(false);

void producer(int id) {
    for (int i = 0; i < 5; ++i) {
        // 模拟任务准备时间
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        {	
            std::lock_guard<std::mutex> lock(mtx); 
            // 生产任务
            int task = id * 10 + i;
            taskQueue.push(task);
            std::cout << "生产者 " << id << " 生产了任务：" << task << std::endl;
        }	// 离开作用域自动解锁
        cv.notify_one();
    }
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        // 等待条件：队列非空或要求退出
        cv.wait(lock, [] {
            return !taskQueue.empty() || shutdown.load();
            });
        // 如果只写cv.wait(lock)可能导致虚假唤醒

        // 如果请求退出且队列为空，则退出
        if (shutdown.load() && taskQueue.empty()) {
            break;
        }

        int task = taskQueue.front();
        taskQueue.pop();

        std::cout << "消费者 " << id << " 处理了任务: " << task << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        lock.unlock();  // 及时释放锁，减少竞争，最小化锁持有时间
    }
}

//int main() {
//    std::cout << "=== 条件变量示例：生产者-消费者模型 ===" << std::endl;
//
//    std::thread p1(producer, 1);
//    std::thread p2(producer, 2);
//    std::thread c1(consumer, 1);
//    std::thread c2(consumer, 2);
//
//    // 等待生产者完成
//    p1.join();
//    p2.join();
//
//    // 设置退出标志并唤醒所有消费者
//    shutdown.store(true);    // atomic操作，线程安全
//    cv.notify_all();         // 通知所有消费者
//
//    // 等待消费者完成
//    c1.join();
//    c2.join();
//
//    std::cout << "=== 程序结束 ===" << std::endl;
//}

#include <memory>
#include <vector>

// ---------- 基类（抽象基类，后面会解释"抽象"）----------
class Shape {
public:
    // 纯虚函数：=0 表示"只声明接口，不给实现"
    virtual void draw() const = 0;
    virtual double area() const = 0;
    virtual ~Shape() = default;   // 虚析构！后面详述为什么必须有
};

// ---------- 派生类 1 ----------
class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    void draw() const override {
        std::cout << "画一个圆，半径 " << r_ << "\n";
    }
    double area() const override { return 3.14159 * r_ * r_; }
};

// ---------- 派生类 2 ----------
class Rect : public Shape {
    double w_, h_;
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    void draw() const override {
        std::cout << "画一个矩形 " << w_ << " x " << h_ << "\n";
    }
    double area() const override { return w_ * h_; }
};

// ---------- 以基类指针/引用操作，多态发挥作用 ----------
void renderAll(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (const auto& s : shapes) {
        s->draw();                     // 动态绑定：运行时决定调用谁
        std::cout << "  面积 = " << s->area() << "\n";
    }
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rect>(2.0, 3.0));
    renderAll(shapes);
}
