/** Implementation file for image quantization
 *
 *  \file ipcv/quantize/quantize.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \co-author Myles Stelling (mcs2868@g.rit.edu)
 *  \date 11 Jun 2022
 */

#include "Quantize.h"

#include <iostream>

using namespace std;

/** Perform uniform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Uniform(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
  // Creating divisor variable for quantization calculation. The divisor is
  // equal to 256 (number of possible greylevels in the image) divided by our
  // quantization levels
  double divisor = 256.0 / quantization_levels;

  // Three for loops to run through the channels, rows, and columns respectively
  for (int channel = 0; channel < 3; channel++) {
    for (int row = 0; row < src.rows; row++) {
      for (int col = 0; col < src.cols; col++) {
        // Creating variables src_val (created to make calculation more
        // readable) and quantized_val (value of each quantized pixel). Both are
        // auto data types to avoid memory problems
        auto src_val = src.at<cv::Vec3b>(row, col)[channel];
        auto quantized_val = src_val / divisor;

        // Setting each found quantized pixel value to each corresponding pixel
        // value in the destination image
        dst.at<cv::Vec3b>(row, col)[channel] = quantized_val;
      }
    }
  }
}

/** Perform improved grey scale quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Igs(const cv::Mat& src, const int quantization_levels, cv::Mat& dst) {
  // Creating divisor variable again as well as a remainder variable to be used
  // in the loops
  int divisor = 256 / quantization_levels;
  int remainder = 0;

  // Three for loops to run through the channels, rows and columns respectively
  for (int channel = 0; channel < 3; channel++) {
    for (int row = 0; row < src.rows; row++) {
      for (int col = 0; col < src.cols; col++) {
        // Creating src_val variable again, this time adding on whatever
        // remainder exists
        int src_val = src.at<cv::Vec3b>(row, col)[channel] + remainder;

        // If statement to ensure resulting quantized value is not outside of
        // the 0 to 255 range of pixel values
        if (src_val > 255) {
          src_val = 255;
        }

        // Creating quantized value again. Same as quantized_val in uniform
        // quantization function. Also finding the remainder of the src_val /
        // divisor and assigning it to the remainder variable
        int quantized_val = src_val / divisor;
        remainder = src_val % divisor;

        // Setting each found quantized pixel value to each corresponding pixel
        // value in the destination image
        dst.at<cv::Vec3b>(row, col)[channel] = quantized_val;
      }
    }
  }
}

namespace ipcv {

bool Quantize(const cv::Mat& src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat& dst) {
  dst.create(src.size(), src.type());

  switch (quantization_type) {
    case QuantizationType::uniform:
      Uniform(src, quantization_levels, dst);
      break;
    case QuantizationType::igs:
      Igs(src, quantization_levels, dst);
      break;
    default:
      cerr << "Specified quantization type is unsupported" << endl;
      return false;
  }

  return true;
}
}
