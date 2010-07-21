#include <iostream>
#include <map>

using namespace std;

namespace vaszauskas {

using namespace std;

class MemoryTracer {

  private:
    struct Record {
        const char *file;
        int line;

        Record(const char *file, int line) : file(file), line(line) {}
    };
    map<void *, Record> allocations;

  public:
    MemoryTracer() {}

    ~MemoryTracer() {
        dump();
    }

    void add(void *p, const char *file, int line) {
        allocations.insert(make_pair(p, Record(file, line) ) );
    }

    void remove(void *p) {
        map<void *, Record>::iterator it = allocations.find(p);
        if (it != allocations.end() ) {
            allocations.erase(it);
        }
    }

    void dump() {
        if (allocations.size() == 0) {
            return;
        }

        cerr << "MEMORY LEAKS " << endl;
        map<void *, Record>::iterator it;

        for (it = allocations.begin(); it != allocations.end(); ++it) {
            cerr << "  " << it->second.file << " at line "
                 << it->second.line << endl;
        }
    }
};

MemoryTracer _memory_tracer;

}  // namespace vaszauskas

void * operator new(size_t size, const char *file, int line) {
    void *p = malloc(size);
    vaszauskas::_memory_tracer.add(p, file, line);
    return p;
}

void * operator new[](size_t size, const char *file, int line) {
    return operator new(size, file, line);
}

void operator delete(void *p) {
    vaszauskas::_memory_tracer.remove(p);
    free(p);
}

void operator delete[](void *p) {
    vaszauskas::_memory_tracer.remove(p);
    free(p);
}

#define new new(__FILE__, __LINE__)
