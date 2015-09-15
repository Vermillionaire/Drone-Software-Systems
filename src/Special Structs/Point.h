
struct Point {
  int x;
  int y;
  int z;

  bool operator==(const Point &other) const {
    return (x == other.x
            && y == other.y
            && z == other.z);
  }
};


struct PointHasher {
  std::size_t operator()(const Point& k) const {
    using std::size_t;
    using std::hash;

  return ((hash<int>()(k.x) << 8
           ^ (hash<int>()(k.y) >> 10)) << 2)
           ^ (hash<int>()(k.z) << 8);
  }
};
