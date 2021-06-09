#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

Scalar FINAL_LINE_COLOR(255, 255, 255);
Scalar WORKING_LINE_COLOR(127, 127, 127);


class RoiSetup {
public:
  string window_name_;
  bool done_roi;
  bool done_rod;
  int npts;
  int ncorners;
  const Point *pts;
  Point current_;
  Point left_upper_corner;
  Point right_bottom_corner;
  Mat img;
  vector<Point> points_;

  RoiSetup(const string window_name, Mat &frame)
  {
    done_roi = false;
    done_rod = false;
    ncorners = 0;
    current_ = Point(0, 0); // Current position, so we can draw the line-in-progress
    img = frame.clone();
  }

  static void onMouse(int event, int x, int y, int flags, void *data)
  {
    RoiSetup *curobj = reinterpret_cast<RoiSetup *>(data);
    if (curobj->done_roi && curobj->done_rod)
    { // Nothing more to do
      return;
    }
    if (!curobj->done_roi)
    {
      if (event == EVENT_MOUSEMOVE)
      {
        // To be able to draw the line-in-progress, so update current mouse position
        curobj->current_ = Point(x, y);
      }
      else if (event == EVENT_LBUTTONDOWN)
      {
        // Left click means adding a point at current position to the list of points
        printf("Adding point #%zu with position(%d,%d) \n", curobj->points_.size(), x, y);
        curobj->points_.push_back(Point(x, y));
        // Initialize or change corners value
        if (curobj->points_.size() == 1)
        {
          curobj->left_upper_corner = Point(x, y);
          curobj->right_bottom_corner = Point(x, y);
        }
        else
        {
          if (x < curobj->left_upper_corner.x)
            curobj->left_upper_corner.x = x;
          if (y < curobj->left_upper_corner.y)
            curobj->left_upper_corner.y = y;
          if (x > curobj->right_bottom_corner.x)
            curobj->right_bottom_corner.x = x;
          if (y > curobj->right_bottom_corner.y)
            curobj->right_bottom_corner.y = y;
        }
      }
      else if (event == EVENT_RBUTTONDOWN)
      {
        // Right click means ending of drawing
        printf("Completing polygon with %zu points \n", curobj->points_.size());
        curobj->done_roi = true;
      }
    }
    else if (!curobj->done_rod)
    {
      if (event == EVENT_MOUSEMOVE)
      {
        // To be able to draw the line-in-progress, so update current mouse position
        if (curobj->ncorners > 0)
        {
          curobj->right_bottom_corner = Point(x, y);
        }
      }
      else if (event == EVENT_LBUTTONDOWN)
      {
        curobj->left_upper_corner = Point(x, y);
        curobj->ncorners++;
      }
      else if (event == EVENT_LBUTTONUP)
      {
        curobj->right_bottom_corner = Point(x, y);
        curobj->ncorners = 0;
      }
      else if (event == EVENT_RBUTTONDOWN)
      {
        printf("Detection area selected\n");
        curobj->done_rod = true;
      }
    }
  }

  void init()
  {
    runRoiSelecting();
    runRodSelecting();
  }

  void runRoiSelecting()
  {
    // Let's create our working window and set a mouse callback to handle events
    window_name_ = "Initialize: selecting region of interest";
    namedWindow(window_name_, WINDOW_KEEPRATIO);
    imshow(window_name_, img);
    waitKey(1);
    setMouseCallback(window_name_, onMouse, this);
    int thickness = 2;
    while (!done_roi)
    {
      Mat img_temp = img.clone();
      if (points_.size() > 0)
      {
        // Draw all the current polygon segments
        pts = (const Point *)Mat(points_).data;
        npts = Mat(points_).rows;
        polylines(img_temp, &pts, &npts, 1, false, FINAL_LINE_COLOR, thickness = thickness);
        // Show what the current segment would look like
        line(img_temp, points_[points_.size() - 1], current_, WORKING_LINE_COLOR, 2.0);
        circle(img, points_[points_.size() - 1], 10, Scalar(0, 255, 0), 1, 8, 0);
      }
      imshow(window_name_, img_temp);
      // Wait 50ms before next iteration or complete
      if (waitKey(50) == 27)
      {
        done_roi = true;
        done_rod = true;
      }
    }
    pts = (const Point *)Mat(points_).data;
    npts = Mat(points_).rows;
    // Finished entering the polygon points
    if (points_.size() > 0)
    {
      polylines(img, &pts, &npts, 1, true, FINAL_LINE_COLOR, thickness = thickness);
      // fillPoly(*imgPtr, &pts, &npts, 1, FINAL_LINE_COLOR);
      imshow(window_name_, img);
    }

    destroyWindow(window_name_);
  }

  void runRodSelecting()
  {
    // Let's create our working window and set a mouse callback to handle events
    window_name_ = "Initialize: selecting region of detection";
    namedWindow(window_name_, WINDOW_KEEPRATIO);
    imshow(window_name_, img);
    waitKey(1);
    setMouseCallback(window_name_, onMouse, this);
    int thickness = 2;

    ncorners = 0;
    while (!done_rod)
    {
      Mat img_temp = img.clone();
      // Choose area of detection
      double alpha = 0.6;
      Mat overlay = img_temp.clone();
      rectangle(overlay, left_upper_corner, right_bottom_corner, FINAL_LINE_COLOR, -1);
      addWeighted(overlay, alpha, img_temp, 1 - alpha, 0, img_temp);
      imshow(window_name_, img_temp);
      if (waitKey(50) == 27)
      {
        done_roi = true;
        done_rod = true;
      }
    }
    
    destroyWindow(window_name_);
  }
};