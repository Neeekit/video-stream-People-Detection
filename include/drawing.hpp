using namespace cv;
using namespace std;

void DrawText(Mat &frame, string text, float size = 0.5)
{
    int left = 0, top = 15;
    int baseLine;
    Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, size, 1, &baseLine);
    top = max(top, textSize.height);
    rectangle(frame, Point(left, top - round(1.5 * textSize.height)), Point(left + round(1.5 * textSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
    putText(frame, text, Point(left, top), FONT_HERSHEY_SIMPLEX, size, Scalar(0, 0, 0), 1);
}

void DrawBox(Mat &frame, vector<string> classes, int classId, float conf, int left, int top, int right, int bottom, string &status)
{
	//Draw detection frame
	if (status == "out")
	{
		rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);
	}
	else
	{
		rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 3);
		DrawText(frame, "Alert", 1.0);
	}
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
	}
}

void DrawPolygonRelativeBoxes(Mat &frame, vector<bbox_t> outs, vector<string> classes, RoiSetup &pd)
{
	//Get all the best detection frame information
	for (int i = 0; i < outs.size(); i++)
	{
		string status = "out";
		if (pointPolygonTest(pd.points_,
							 Point2f(outs[i].x + outs[i].w / 2 + pd.left_upper_corner.x,
									 outs[i].y + outs[i].h + pd.left_upper_corner.y),
							 true) > 0)
		{
			status = "in";
		}
		DrawBox(frame, classes, outs[i].obj_id, outs[i].prob, outs[i].x + pd.left_upper_corner.x,
				outs[i].y + pd.left_upper_corner.y,
				outs[i].x + outs[i].w + pd.left_upper_corner.x,
				outs[i].y + outs[i].h + pd.left_upper_corner.y, status);
	}
}

void drawGraphics(Mat &frame, vector<bbox_t> outs, vector<string> classes, RoiSetup &roiSetup)
{
	DrawPolygonRelativeBoxes(frame, outs, classes, roiSetup);
	polylines(frame, &roiSetup.pts, &roiSetup.npts, 1, true, FINAL_LINE_COLOR);
}