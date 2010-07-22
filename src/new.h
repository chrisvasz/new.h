#include <iostream>
#include <map>
#include <set>
#include <sstream>

namespace vaszauskas {

using namespace std;

/**
 * @brief  A tracker class based on an STL map that tracks memory allocations.
 */
class MemoryTracker {

  private:
    struct Record {
        const char *file;
        int line;

        Record(const char *file, int line) : file(file), line(line) {}

        string str() {
            stringstream result;
            result << file << " at line " << line;
            return result.str();
        }
    };

    map<void *, Record> allocations;
    map<string, map<int, int> > data;

  public:
    MemoryTracker() {}

    ~MemoryTracker() {
        report();
    }

    void add(void *p, const char *file, int line) {
        Record r = Record(file, line);
        allocations.insert(make_pair(p, r) );
        data[r.file][r.line]++;
    }

    void remove(void *p) {
        map<void *, Record>::iterator it = allocations.find(p);
        if (it != allocations.end() ) {
            // guaranteed to find a corresponding entry in data
            map<string, map<int, int> >::iterator sit;
            sit = data.find(it->second.file);

            // found the corresponding entry in data; now find the internal
            // entry in the map inside sit
            map<int, int>::iterator mit;
            mit = sit->second.find(it->second.line);

            // found the internal entry; erase it if it is 0.
            if (--mit->second == 0) {
                sit->second.erase(mit);
            }
            allocations.erase(it);
        }
    }

    void report() {
        if (allocations.size() == 0) {
            return;
        }

        cerr << "============" << endl;
        cerr << "MEMORY LEAKS" << endl;
        map<string, map<int, int> >::iterator it;
        map<int, int>::iterator mit;
        for (it = data.begin(); it != data.end(); ++it) {
            cerr << "  " << it->first << endl;
            for (mit = it->second.begin(); mit != it->second.end(); ++mit) {
                cerr << "    " << mit->second << " leak(s) at line " << mit->first << endl;
            }
        }
    }
};

MemoryTracker _memory_tracker;

}  // namespace vaszauskas

void * operator new(size_t size, const char *file, int line) {
    void *p = malloc(size);
    vaszauskas::_memory_tracker.add(p, file, line);
    return p;
}

void * operator new[](size_t size, const char *file, int line) {
    return operator new(size, file, line);
}

void operator delete(void *p) {
    vaszauskas::_memory_tracker.remove(p);
    free(p);
}

void operator delete[](void *p) {
    operator delete(p);
}

#define new new(__FILE__, __LINE__)  // replace global new with ours

