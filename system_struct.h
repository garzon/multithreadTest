//Each probe has its own queue.Each element is a object shared_data.


#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class options
{
public:
    class opt_input
    {
    public:
    };
    class opt_track
    {
    public:
    };
    class opt_face_detection
    {
    public:
    };
    class opt_person_detection
    {
    public:
    };
    class opt_face_recognition
    {
    public:
    };
    class opt_person_recognition
    {
    public:
    };

    opt_input input;
    opt_track track;
    opt_face_detection face_d;
    opt_face_recognition face_r;
    opt_person_detection person_d;
    opt_person_recognition person_r;
};

class mat:public Mat
{
 public:
    using Mat::Mat;
    mat():Mat(){}
    mat(const Mat &t):Mat(t){}
    mat(Mat &&t):Mat(t){}
};

class shared_data  //one frame multi bbox
{
public:
    class info
    {
    public:
        time_t input_stamp; //time stamp
        int input_pos; //which probe
        info(time_t _stamp=0,int  _pos=-1):
            input_stamp(_stamp),
            input_pos(_pos)
        {}
        //no ~info
    };
    class bbox:public Rect_<int>
    {
        public:
        using Rect_::Rect_;
        double prob{0.0}; //confidence probability 0.0~1.0
        int result_label{-1};//the label of this region, means which person in this region
        int type_label{0};//this bbox is for 1 face, 2 person or 3 tracking, 0 init
        bbox():Rect(){}
        bbox(const Rect_<int> &t):Rect_(t){}
        bbox(Rect_<int> &&t):Rect_(t){}
        //no ~bbox
    };
    class frame
    {
    public:
        mat image; // origin image
        vector<bbox> im_ROI; //valued region
        //no ~frame
    };
    class flags
    {
    public:
        int track_flag, detection_flag, output_flag, debug_flag;//0  do nothing, 1 running
        flags(int _track_flag=1, int _detection_flag=1,int  _output_flag=1,int  _debug_flag=1):
            track_flag(_track_flag),
            detection_flag(_detection_flag),
            output_flag(_output_flag),
            debug_flag(_debug_flag)
        {}

        //no ~flags
    };

    info im_info;
    frame im_data;
    flags im_flags;
    vector<bbox> im_boxes;

    shared_data(){}

    shared_data(const info &_im_info,const frame &_im_data,const flags & _im_flags):
        im_info(_im_info), im_data(_im_data), im_flags(_im_flags)
    {}

    shared_data(const shared_data &t)=delete;
    shared_data &operator =(const shared_data &t)=delete;

    shared_data(shared_data &&t){
        im_info=t.im_info;
        im_data=t.im_data;
        im_flags=t.im_flags;
        im_boxes=move(t.im_boxes);
    }

    shared_data & operator = (shared_data &&t){
        im_info=t.im_info;
        im_data=t.im_data;
        im_flags=t.im_flags;
        im_boxes=move(t.im_boxes);
        return *this;
    }

};
