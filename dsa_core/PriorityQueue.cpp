#include "PriorityQueue.h"
#include <iostream>
#include <string>
using namespace std;

PriorityQueue::PriorityQueue(int first_capacity = 300000) {
    capacity = first_capacity;
    hangCho = new WaitlistEntry[capacity];
    size = 0;
    front = 0;
    rear = -1;
}

PriorityQueue::~PriorityQueue() {
    delete[] hangCho;
}

void PriorityQueue::resize() {
    capacity *= 2;
    WaitlistEntry* tmp = new WaitlistEntry[capacity];
    for (int i = 0; i < size; i++) {
        tmp[i] = hangCho[i];
    }
    delete[] hangCho;
    hangCho = tmp;
}

bool PriorityQueue::uuTienHon(const DSHangCho& a, const DSHangCho& b) const {

}

void PriorityQueue::EnQueue(WaitlistEntry* &hangCho) {

}

void PriorityQueue::hienThi() const {

}