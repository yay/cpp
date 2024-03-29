#include <print>
#include <atomic>
#include <numeric>
#include <algorithm>
#include <functional>
#include <memory_resource>
#include <thread>

using MemoryPool = std::pmr::monotonic_buffer_resource;

struct Node
{
    Node *l;
    Node *r;

    int check() const
    {
        if (l)
        {
            return l->check() + 1 + r->check();
        }
        else
        {
            return 1;
        }
    }
};

Node *make(const int d, MemoryPool &store)
{
    Node *root = static_cast<Node *>(store.allocate(sizeof(Node)));
    if (d > 0)
    {
        root->l = make(d - 1, store);
        root->r = make(d - 1, store);
    }
    else
    {
        root->l = root->r = nullptr;
    }
    return root;
}

int run_parallel(unsigned depth, int iterations, unsigned workers = std::thread::hardware_concurrency())
{
    std::vector<std::thread> threads;
    threads.reserve(workers);

    std::atomic_int counter = iterations;
    std::atomic_int output = 0;

    for (unsigned i = 0; i < workers; ++i)
    {
        threads.push_back(std::thread([&counter, depth, &output]
                                      {
            std::pmr::unsynchronized_pool_resource upperPool;
            MemoryPool pool {4000, &upperPool};
            int checksum = 0;

            while (--counter >= 0) {
                Node* a = make(depth, pool);
                checksum += a->check();
                pool.release();
            }

            output += checksum; }));
    }

    for (unsigned i = 0; i < workers; ++i)
    {
        threads[i].join();
    }

    return output;
}

constexpr auto MIN_DEPTH = 4;

int main(int argc, char *argv[])
{
    const int max_depth = std::max(MIN_DEPTH + 2, (argc == 2 ? atoi(argv[1]) : 10));
    const int stretch_depth = max_depth + 1;

    // Alloc then dealloc stretch-depth tree.
    {
        MemoryPool store;
        Node *c = make(stretch_depth, store);
        std::println("stretch tree of depth {}\t check: {}", stretch_depth, c->check());
    }

    MemoryPool long_lived_store;
    Node *long_lived_tree = make(max_depth, long_lived_store);

    for (int d = MIN_DEPTH; d <= max_depth; d += 2)
    {
        const int iterations = 1 << (max_depth - d + MIN_DEPTH);
        auto const c = run_parallel(d, iterations);

        std::println("{}\t trees of depth  {}\t check: {}", iterations, d, c);
    }

    std::println("long lived tree of depth {}\t check: {}", max_depth, long_lived_tree->check());

    return 0;
}
