#include <iostream>

#include "FolderReader.h"
//#include "kdTree.h"
#include "quantizer.h"
#include "pathfinder.h"
#include "pathmerger.h"
#include "markingdetector.h"

#include <omp.h>

#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/TransformStamped.h>

#include <eigen3/Eigen/Eigen>

#include <fstream>

using namespace std;
using namespace sensor_msgs;
using namespace geometry_msgs;
using namespace Eigen;

int MarkingDetector::markID = 0;

void storePly(string folderName, string fileName, BBoxPC &lane)
{
    ofstream ofs(folderName + "/" + fileName + ".ply");
    ofs << R"(ply
format ascii 1.0
comment VCGLIB generated
element vertex )";
    ofs << lane.size() << endl;
    ofs << R"(property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
element face 0
property list uchar int vertex_indices
end_header
)";
    for (auto && p : lane)
    {
        ofs << p[0] << " " << p[1] << " " << p[2] << " " << (int)p.color[0] << " " << (int)p.color[1] << " " << (int)p.color[2] << endl;
    }
    ofs.close();
}


int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        cout << "Please provide folder to the bagfiles: ./lanes_detection ./path/to/folder" << endl;
        return 0;
    }
    FolderReader<> fr(argv[1]);
    //static_assert(sizeof(Point) == 24);

    system("mkdir output");

    map<string, string> topics = {
        {"cont", "/LanePoints/cont_lane"},
        {"curb", "/LanePoints/curbstone"},
        {"dash", "/LanePoints/dash_lane"},
        {"stop", "/LanePoints/stop_lane"}
        };

        map<string, MarkingDetector*> processors; // use Quantizer, PathFinder or PathMerger for debugging these classes
        processors["cont"] = new MarkingDetectorCont(LANEW);
        processors["curb"] = new MarkingDetectorCurb(LANEW);
        processors["dash"] = new MarkingDetectorDash(LANEW);
        processors["stop"] = new MarkingDetectorStop(0.5f*LANEW);

        assert(topics.size() == processors.size());

    while (true)
    {
        string fileName = fr.getNext("bag");
        string baseName = basename(&fileName.c_str()[0]);

        if (fileName.empty())
            break;


//#pragma omp parallel // KB: probably will not work if rosbag is open in threads, other solution for multithreding is to be found
        //for (int i = 0; i < topics.size(); ++i)
        for (auto && t : topics)
        {
            // auto it = topics.begin();
            // advance(it, i);
            // auto t = *it;

            rosbag::Bag bag;
            bag.open(fileName);
            int count = 0;
            auto && messages = rosbag::View(bag);
            deque<BBoxPC> lanes; // lanes is not the best name for the container, some other name should be used (paths, markings, points?)

            string outfolder = "./output/" + baseName  + "/" + t.first;

            system(string("mkdir -p " + outfolder).c_str());

            auto processPC = [&]()
            {
                BBoxPC flatLane; map<int, BBoxPC> lanesmap;
                // flatten the lanes into one array:
                for (auto && l : lanes) copy(l.begin(), l.end(), back_inserter(flatLane));

                processors[t.first]->process(flatLane, lanesmap); // if this is commented the raw data will be stored (bunch of PCs from multiple messages)

                if (!flatLane.empty())
                    storePly(outfolder, to_string(count++), flatLane);
            };

            for (rosbag::MessageInstance const &msg : messages)
            {
                if (msg.getTopic() != t.second)
                    continue;

                BBoxPC lane;

                auto pc = msg.instantiate<PointCloud2>();
                std::vector<uint8_t> data = pc->data;
                for (int i = 0; i < data.size(); i += pc->point_step)
                {
                    Point point(count);
                    memcpy(&point, &data[i], pc->point_step); // the id will not be overriten
                    lane.push_back(point);
                }

                // storePly(baseName, t.first, to_string(count++), lane); // could be used for debugging
                // continue;

                BBoxPC::removeOutliers(lane);
                if (lane.size() < 2) continue;

                // sliding window principle. Add into lanes until the bboxes of front and end do not cross:
                if (!lanes.empty() && !lanes.front().bbox.crossing(lane.bbox))
                {
                    processPC();
                    size_t s = lanes.size();
                    // Remove half of the elements from the from of queue:
                    while (!lanes.empty() && lanes.size() > 0.5*s) lanes.pop_front();
                }
                else lanes.push_back(lane);
            }
            // Process the remaining pc (still in lanes):
            processPC();

            // for Lanes aggregator - there still not aggregated lanes sitting there, get them:
            BBoxPC flatLane; map<int, BBoxPC> lanesmap;
            processors[t.first]->getResults(flatLane, lanesmap, true);

            if (!flatLane.empty())
                storePly(outfolder, to_string(count++), flatLane);

            bag.close();
        }
    }

    return 0;
}