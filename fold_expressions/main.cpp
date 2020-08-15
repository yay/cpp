#include <iostream>
#include <vector>

struct Rect {
    int width;
    int height;

    friend std::ostream& operator<<(std::ostream& out, const Rect& rect) {
        out << "(" << std::to_string(rect.width) << ", " << std::to_string(rect.height) << ")";
        return out;
    }
};

template <class... Rects>
Rect add_rects(const Rects&... rects) {
    const auto width = (rects.width + ...);
    const auto height = std::max({rects.height...});
    return {width, height};
}

int main(int, char**) {
    std::vector<Rect> rects{{200, 100}, {800, 1000}, {1024, 768}};
    for (auto& rect : rects) {
        std::cout << rect << std::endl;
    }
    std::cout << add_rects(rects[0], rects[1], rects[2]) << std::endl;
}
