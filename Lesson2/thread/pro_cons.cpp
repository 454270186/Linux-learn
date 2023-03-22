#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
using namespace std;

const int buffer_size = 20;

mutex mtx;
condition_variable que_not_full;
condition_variable que_not_empty;
queue<int> q;

void produce() {
    while (1) {
        {
            unique_lock<mutex> lock(mtx);
            que_not_full.wait(lock, [&](){return q.size() < buffer_size; });
            int num = rand() % 1000;
            q.push(num);
            cout << "add node, num: " << num << ", curren queue size: "<< q.size() << endl;  
        }
        que_not_empty.notify_one();
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void consume() {
    while (1) {
        {
           unique_lock<mutex> lock(mtx);
            que_not_empty.wait(lock, [&](){return q.size() > 0;});
            int num = q.front();
            q.pop();
            cout << "delete node, num: " << num << ", curren queue size: "<< q.size() << endl; 
        }
        que_not_full.notify_one();
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
}

int main() {
    vector<thread> producer;
    vector<thread> consumer;

    for (int i = 0; i < 10; i++) {
        producer.push_back(thread(produce));
    }

    for (int i = 0; i < 30; i++) {
        consumer.push_back(thread(consume));
    }

    for (int i = 0; i < producer.size(); i++) {
        producer[i].join();
    }

    for (int i = 0; i < consumer.size(); i++) {
        consumer[i].join();
    }
}