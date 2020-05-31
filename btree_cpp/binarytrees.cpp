#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <apr_pools.h>

// Source:
// http://benchmarksgame.alioth.debian.org/u64q/program.php?test=binarytrees&lang=gpp&id=9

// To run use:
// time ./binarytrees 21

// To properly build manually on macOS we need to install GCC:
// # certain flags won't work with clang:
// # clang: error: unsupported option '-fopenmp'
// brew install gcc
// g++-8 -c -g -pipe -O3 -fomit-frame-pointer -march=native -fopenmp -I/usr/include/apr-1 -o binarytrees.o binarytrees.cpp && g++-8 binarytrees.o -o binarytrees -fopenmp -lapr-1

// Results on macOS 10.12.6, late 2013 15" rMPB:
// real	0m1.364s
// user	0m6.588s
// sys	0m0.125s

// Also might want to see: https://iscinumpy.gitlab.io/post/omp-on-high-sierra/

const size_t    LINE_SIZE = 64;

class Apr {
    public:
        Apr()
        {
            apr_initialize();
        }

        ~Apr()
        {
            apr_terminate();
        }
};

struct Node {
    Node *l, *r;

    int check() const {
        if (l)
            return l->check() + 1 + r->check();
        else
            return 1;
    }
};

class NodePool {
    public:
        NodePool()
        {
            apr_pool_create_unmanaged(&pool);
        }

        ~NodePool()
        {
            apr_pool_destroy(pool);
        }

        Node* alloc()
        {
            return (Node *)apr_palloc(pool, sizeof(Node));
        }

        void clear()
        {
            apr_pool_clear(pool);
        }

    private:
        apr_pool_t* pool;
};

Node *make(int depth, NodePool &store) {
    Node* root = store.alloc();

    if (depth > 0) {
        root->l = make(depth - 1, store);
        root->r = make(depth - 1, store);
    } else {
        root->l = root->r = 0;
    }

    return root;
}

int main(int argc, char *argv[]) {
    Apr apr;
    int min_depth = 4;
    int max_depth = std::max(min_depth + 2,
                             (argc == 2 ? atoi(argv[1]) : 10));
    int stretch_depth = max_depth + 1;

    // Alloc then dealloc stretchdepth tree
    {
        NodePool store;
        Node *c = make(stretch_depth, store);
        std::cout << "stretch tree of depth " << stretch_depth << "\t "
                  << "check: " << c->check() << std::endl;
    }

    NodePool long_lived_store;
    Node *long_lived_tree = make(max_depth, long_lived_store);

    // buffer to store output of each thread
    char *outputstr = (char*)malloc(LINE_SIZE * (max_depth +1) * sizeof(char));

    #pragma omp parallel for
    for (int d = min_depth; d <= max_depth; d += 2) {
        int iterations = 1 << (max_depth - d + min_depth);
        int c = 0;

        // Create a memory pool for this thread to use.
        NodePool store;

        for (int i = 1; i <= iterations; ++i) {
            Node *a = make(d, store);
            c += a->check();
            store.clear();
        }

        // each thread write to separate location
        sprintf(outputstr + LINE_SIZE * d, "%d\t trees of depth %d\t check: %d\n",
           iterations, d, c);
    }

    // print all results
    for (int d = min_depth; d <= max_depth; d += 2)
        printf("%s", outputstr + (d * LINE_SIZE) );
    free(outputstr);

    std::cout << "long lived tree of depth " << max_depth << "\t "
              << "check: " << (long_lived_tree->check()) << "\n";

    return 0;
}