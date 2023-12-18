#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

class interval {
  private:
    double min, max;

  public:
    interval() : min(std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {} // Default interval is empty

    interval(double _min, double _max) : min(_min), max(_max) {}

    double getMin() const {
        return min;
    }

    void setMin(double _min) {
        min = _min;
    }

    double getMax() const {
        return max;
    }

    void setMax(double _max) {
        max = _max;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
const interval interval::universe = interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

#endif