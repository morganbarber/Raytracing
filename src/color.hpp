#ifndef COLOR_H #define COLOR_H

#include "vec3.h"

#include #include

using color = vec3;

inline double linear_to_gamma(double linear_component) {
  return std::sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples.
  auto scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Apply a linear to gamma transform for gamma 2
  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  // Write the translated [0,255] value of each color component.
  static constexpr double intensity_min = 0.0;
  static constexpr double intensity_max = 0.999;
  static constexpr double intensity_range = intensity_max - intensity_min;
  out << static_cast<int>(256 * (intensity_min + intensity_range * r)) << ' '
      << static_cast<int>(256 * (intensity_min + intensity_range * g)) << ' '
      << static_cast<int>(256 * (intensity_min + intensity_range * b)) << '\n';

}

#endif