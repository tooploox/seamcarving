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
    Sobel(greyMat, gradX, CV_16S, 1, 0);
    Sobel(greyMat, gradY, CV_16S, 0, 1);
    addWeighted(gradX, 0.5, gradY, 0.5, 0.0, output);

    imwrite(out, output);
    //    printImage(output);
    return output;
}

void minimalEnergyToBottom(const cv::Mat& Energy, cv::Mat& MinEnergy)
{
    // copy bottom row
    Energy.row(Energy.rows - 1).copyTo(MinEnergy.row(MinEnergy.rows - 1));

    cv::Ptr<cv::Formatter> fmt = cv::Formatter::get(cv::Formatter::FMT_DEFAULT);
    fmt->set32fPrecision(2);

    const auto penultimate = Energy.rows - 2;
    const auto cols = MinEnergy.cols;
    const auto lastColumn = MinEnergy.cols - 1;
    // process bottom up from penultimate
    for (int r = penultimate; r >= 0; r--)
    {
        for (int c = 0; c < cols; ++c)
        {
            std::vector<float> children;
            if (c != 0 && c != lastColumn)
            {
                children = {MinEnergy.at<float>(r + 1, c - 1), MinEnergy.at<float>(r + 1, c),
                            MinEnergy.at<float>(r + 1, c + 1)};
            }
            else if (c == 0)
            {
                children = {MinEnergy.at<float>(r + 1, c), MinEnergy.at<float>(r + 1, c + 1)};
            }
            else
            {
                children = {MinEnergy.at<float>(r + 1, c - 1), MinEnergy.at<float>(r + 1, c)};
            }

            const auto minimalChild = *std::min_element(children.cbegin(), children.cend());
            MinEnergy.at<float>(r, c) = Energy.at<float>(r, c) + minimalChild;
            std::cout << std::dec << "col: " << c << " row: " << r << " min: " << minimalChild << " from ";
            for (const auto child : children)
            {
                std::cout << child << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}
