#include <string>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cassert>

using namespace std;

static void *auto_sort(void *ptr);

class List {
private:
    friend void *auto_sort(void*);

    class Node {
    private:
        friend class List;
    public:
        string data;
        Node *next;
        Node *prev;

        static int compare(Node *n1, Node *n2) {
            return n1->data.compare(n2->data);
        }

        static void swap(Node *n1, Node *n2) {
            n1->data.swap(n2->data);
        }

        Node(string data, Node *next, Node *prev) :
        data(data), next(next), prev(prev) {

        }
    };

    Node *getNodeIndexOf(int index) {
        int i = 0;
        assert(index >= 0);
        Node *n = head->next;

        for (;; n = n->next, ++i) {
            if (i == index) {
                break;
            }
        }
        return n;
    }

    void sort_list() {
        if (list_size <= 1)
            return;

        for (int i = list_size - 1; i > 0; --i) {
            for (int j = 0; j < i; ++j) {
                Node *n1 = getNodeIndexOf(j);
                Node *n2 = getNodeIndexOf(j + 1);
                if (Node::compare(n1, n2) > 0)
                    Node::swap(n1, n2);
            }
        }
    }

    static void *auto_sort(void *ptr) {
        List *list = static_cast<List*> (ptr);
        const static int SLEEP_TIME = 1;
        for (;;) {
            sleep(SLEEP_TIME);

            if (list->head == NULL) {
                break;
            }
            pthread_rwlock_wrlock(&list->rwlock);

            list->sort_list();

            pthread_rwlock_unlock(&list->rwlock);
        }

        return NULL;
    }


    pthread_t sort_tid;
    Node *head;
    int list_size;
    mutable pthread_rwlock_t rwlock;
public:

    List() : list_size(0) {
        head = new Node("", NULL, NULL);
        head->next = head;
        head->prev = head;

        pthread_rwlock_init(&rwlock, NULL);
        pthread_create(&sort_tid, NULL, &auto_sort, this);
    }

    ~List() {

        pthread_rwlock_wrlock(&rwlock);
        for (Node *n = head->next; n != head;) {
            Node *t = n->next;
            delete n;
            n = t;
        }

        delete head;
        list_size = 0;
        head = NULL;

        pthread_rwlock_unlock(&rwlock);

        pthread_join(sort_tid, NULL);
        pthread_rwlock_destroy(&rwlock);
    }

    int size() const {
        pthread_rwlock_rdlock(&rwlock);
        return list_size;
        pthread_rwlock_unlock(&rwlock);
    }

    void push_front(string s) {
        pthread_rwlock_wrlock(&rwlock);

        Node *n = new Node(s, head->next, head);
        head->next->prev = n;
        head->next = n;

        list_size++;

        pthread_rwlock_unlock(&rwlock);
    }

    void print_list() const {
        pthread_rwlock_rdlock(&rwlock);

        int counter = 0;

        for (Node *n = head->next; n != head; n = n->next) {
            cout << "Entry " << counter << "\t" << n->data << endl;
            counter++;
        }

        pthread_rwlock_unlock(&rwlock);
    }
};

int main(int argc, char *argv[]) {

    List list;
    cout << "Input lines, please:" << endl;
    string s;
    for (;;) {
        getline(cin, s);

        if (cin.eof())
            break;

        if (s.empty()) {
            list.print_list();
        } else {
            list.push_front(s);
        }
    }
    pthread_exit(NULL);
}