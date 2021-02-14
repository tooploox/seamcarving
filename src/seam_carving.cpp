#include "seam_carving.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat seamCarving(const cv::Mat& image, const cv::Size& out_size)
{
    (void)out_size;

    return image;
}

void grey(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    imwrite(out, greyMat);
}

void edges(const cv::Mat& image, fs::path out)
{
    cv::Mat edges;
    Canny(image, edges, 10, 100);
    imwrite(out, edges);
}

void sobelX(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradX;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradX, CV_16S, 1, 0);

    imwrite(out, gradX);
}

void sobelY(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradY;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradY, CV_16S, 0, 1);

    imwrite(out, gradY);
}

cv::Mat sobelXY(const cv::Mat& image, fs::path out)
{
    cv::Mat greyMat;
    cv::Mat gradX;
    cv::Mat gradY;
    cv::Mat output;

    cvtColor(image, greyMat, cv::COLOR_BGR2GRAY);
    Sobel(greyMat, gradX, CV_64F, 1, 0);
    Sobel(greyMat, gradY, CV_64F, 0, 1);
    addWeighted(gradX, 0.5, gradY, 0.5, 0.0, output);

    imwrite(out, output);
    return output;
}

void drawLeastEnergyCurve(cv::Mat& castle, const cv::Mat& minEnergy, const cv::Mat& arrows)
{
    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;

    minMaxLoc(minEnergy.row(0), &minVal, &maxVal, &minLoc, &maxLoc);

    std::cout << "minVal " << minVal << " maxVal " << maxVal << " minLoc " << minLoc << " maxLoc " << maxLoc << "\n";
    for (int counter = 0; counter < arrows.rows; ++counter)
    {
        cv::Vec3b& color = castle.at<cv::Vec3b>(minLoc);
        color[0] = 0;
        color[1] = 0;
        color[2] = 255;

        if (arrows.at<float>(minLoc) == -1)
        {
            minLoc.x -= 1;
            minLoc.y += 1;
        }
        else if (arrows.at<float>(minLoc) == 0)
        {
            minLoc.y += 1;
        }
        else
        {
            minLoc.x += 1;
            minLoc.y += 1;
        }
        std::cout << " minLoc " << minLoc << "\n";
    }
}