#include <iostream>
#include <stdlib.h>
#include <stdio.h>

// Source:
// http://benchmarksgame.alioth.debian.org/u64q/program.php?test=binarytrees&lang=gpp&id=9

// To run use:
// time ./binarytrees 21

const size_t LINE_SIZE = 64;

struct Node {
  Node *l, *r;

  int check() const {
    if (l)
      return l->check() + 1 + r->check();
    else
      return 1;
  }
};

Node *make(int d) {
  Node *root = new Node;

  if (d > 0) {
    root->l = make(d - 1);
    root->r = make(d - 1);
  } else {
    root->l = root->r = 0;
  }

  return root;
}

void destroy(Node *root) {
  if (root) {
    destroy(root->l);
    destroy(root->r);
    delete root;
  }
}

int main(int argc, char *argv[]) {
  int min_depth = 4;
  int max_depth = std::max(min_depth + 2,
                           (argc == 2 ? atoi(argv[1]) : 10));
  int stretch_depth = max_depth + 1;

  // Alloc then dealloc stretchdepth tree
  {
    Node *c = make(stretch_depth);
    std::cout << "stretch tree of depth " << stretch_depth << "\t "
              << "check: " << c->check() << std::endl;
    destroy(c);
  }

  Node *long_lived_tree = make(max_depth);

  // buffer to store output of each thread
  char *outputstr = (char *)malloc(LINE_SIZE * (max_depth + 1) * sizeof(char));

  #pragma omp parallel for
  for (int d = min_depth; d <= max_depth; d += 2) {
    int iterations = 1 << (max_depth - d + min_depth);
    int c = 0;

    for (int i = 1; i <= iterations; ++i) {
      Node *a = make(d);
      c += a->check();
      destroy(a);
    }

    // each thread write to separate location
    sprintf(outputstr + LINE_SIZE * d, "%d\t trees of depth %d\t check: %d\n",
            iterations, d, c);
  }

  // print all results
  for (int d = min_depth; d <= max_depth; d += 2)
    printf("%s", outputstr + (d * LINE_SIZE));
  free(outputstr);

  std::cout << "long lived tree of depth " << max_depth << "\t "
            << "check: " << (long_lived_tree->check()) << "\n";

  destroy(long_lived_tree);

  return 0;
}