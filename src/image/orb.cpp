/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
#include "vision-artificial/image/orb.hpp"

#include <cstddef>
#include <cstdio>

#include <vector>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
#define MAX_FEATURES	(50000)
#define GOOD_MATCH_P	(0.25)


/******************************************************************************
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs / unions *****************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	void	img_orb_align	(const class cv::Mat  *img_0,
				class cv::Mat  *img_1);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
void	img_orb_act	(const class cv::Mat  *img_ref,
			class cv::Mat  *imgptr, int action)
{
	switch (action) {
	case IMG_ORB_ACT_ALIGN:
		img_orb_align(img_ref, imgptr);
		break;
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	void	img_orb_align	(const class cv::Mat  *img_0,
				class cv::Mat  *img_1)
{
	class std::vector <class cv::KeyPoint>		keypoints_0;
	class std::vector <class cv::KeyPoint>		keypoints_1;
	class cv::Mat					descriptors_0;
	class cv::Mat					descriptors_1;
#if (CV_VERSION_EPOCH == 2)
	class cv::ORB					orb;
#elif (CV_VERSION_EPOCH == 3)
	class cv::Ptr <class cv::Feature2D>		orb;
#endif
	class std::vector <struct cv::DMatch>		matches;
	class cv::Ptr <class cv::DescriptorMatcher>	matcher;
	ptrdiff_t					good_matches;
	class cv::Mat					img_matches;
	class std::vector <class cv::Point_ <float>>	points_0;
	class std::vector <class cv::Point_ <float>>	points_1;
	ptrdiff_t					size;
	class cv::Mat					img_hg;
	class cv::Mat					img_align;

	/* Detect ORB features & compute descriptors */
#if (CV_VERSION_EPOCH == 2)
	orb(*img_0, cv::Mat(), keypoints_0, descriptors_0);
	orb(*img_1, cv::Mat(), keypoints_1, descriptors_1);
#elif (CV_VERSION_EPOCH == 3)
	orb	= cv::ORB::create(MAX_FEATURES);
	orb->detectAndCompute(*img_0, cv::Mat(), keypoints_0, descriptors_0);
	orb->detectAndCompute(*img_1, cv::Mat(), keypoints_1, descriptors_1);
#endif

	/* Match structures */
	matcher	= cv::DescriptorMatcher::create("BruteForce-Hamming");
	matcher->match(descriptors_1, descriptors_0, matches, cv::Mat());

	/* Sort matches by score */
	std::sort(matches.begin(), matches.end());

	/* Remove not so good matches */
	good_matches	= GOOD_MATCH_P * matches.size();
	matches.erase(matches.begin() + good_matches, matches.end());

	/* Draw top matches */
	cv::drawMatches(*img_1, keypoints_1, *img_0, keypoints_0, matches,
								img_matches);
	cv::imwrite("matches.jpg", img_matches);

	/* Extract location of good matches */
	size	= matches.size();
	for (ptrdiff_t i = 0; i < size; i++) {
		points_1.push_back(keypoints_1[matches[i].queryIdx].pt);
		points_0.push_back(keypoints_0[matches[i].trainIdx].pt);
	}

	/* Find homography */
	img_hg	= cv::findHomography(points_1, points_0, CV_RANSAC);

	/* Use homography to warp image */
	cv::warpPerspective(*img_1, img_align, img_hg, img_0->size());

	/* Write img_align into img_1 */
	*img_1	= img_align;
	img_align.release();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
