#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "spn.h"

#define ROUNDS 5

using namespace cv;

void toMatlab(uint8_t to, std::vector<uint8_t> &what); // to test statistical properties

int main()
{
	Mat image = imread(samples::findFile("peper.jpg"), IMREAD_GRAYSCALE);

	if (image.empty())
	{
		std::cout << "Can't open image!" << std::endl;
		return 1;
	}
	imshow("Original image", image);

	std::vector<uint8_t> pixels;
	for (size_t i = 0; i < image.cols; ++i)
		for (size_t j = 0; j < image.rows; ++j)
			pixels.push_back(image.at<uint8_t>(Point(i, j)));

	// toMatlab(0, pixels);

	SP net(ROUNDS);

	// encrypt

	net.encrypt(pixels);

	Mat encrypted_image = Mat::zeros(image.size(), image.type());
	for (size_t i = 0; i < encrypted_image.cols; ++i)
		for (size_t j = 0; j < encrypted_image.rows; ++j)
			encrypted_image.at<uint8_t>(Point(i, j)) = pixels[i * encrypted_image.rows + j];

	imshow("Encrypted image", encrypted_image);

	// toMatlab(1, pixels);

	// decrypt

	net.decrypt(pixels);

	Mat decrypted_image = Mat::zeros(image.size(), image.type());
	for (size_t i = 0; i < decrypted_image.cols; ++i)
		for (size_t j = 0; j < decrypted_image.rows; ++j)
			decrypted_image.at<uint8_t>(Point(i, j)) = pixels[i * decrypted_image.rows + j];

	imshow("Decrypted image", decrypted_image);

	waitKey(0);
	return 0;
}

void toMatlab(uint8_t to, std::vector<uint8_t> &what)
{
	std::ofstream out;
	out.open("C:\\do\\matlab\\latticework " + std::to_string(to) + ".txt");
	if (out.is_open())
	{
		for (size_t i = 0; i < what.size() - 1; ++i)
			out << (uint16_t)what[i] << " " << (uint16_t)what[i + 1] << std::endl;
		out.close();
	}

	out.open("C:\\do\\matlab\\pixels " + std::to_string(to) + ".txt");
	if (out.is_open())
	{
		for (size_t i = 0; i < what.size(); ++i)
			out << (uint16_t)what[i] << std::endl;
		out.close();
	}
}