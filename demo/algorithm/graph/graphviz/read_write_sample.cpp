#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <string>
#include <sstream>

int main(const int argc, const char *argv[])
{
	typedef boost::property<boost::vertex_name_t, std::string,
		boost::property<boost::vertex_color_t, float>> vProperty;
	typedef boost::property<boost::edge_weight_t, double> eProperty;
	typedef boost::property<boost::graph_name_t, std::string> gProperty;
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::directedS, vProperty, eProperty, gProperty> Graph;

	// Construct an empty graph and prepare the dynamic_property_maps.
	Graph g(0);
	boost::dynamic_properties dp;

	boost::property_map<Graph, boost::vertex_name_t>::type name = get(boost::vertex_name, g);
	dp.property("node_id", name);

	//boost::property_map<Graph, boost::vertex_color_t>::type mass = get(boost::vertex_color, g);
	//dp.property("mass", mass);

	//boost::property_map<Graph, boost::edge_weight_t>::type weight = get(boost::edge_weight, g);
	//dp.property("weight", weight);

	// Use ref_property_map to turn a g property into a property map
	boost::ref_property_map<Graph*, std::string> gname(boost::get_property(g, boost::graph_name));
	dp.property("name", gname);

	if (boost::read_graphviz(std::cin, g, dp, "node_id"))
		  boost::write_graphviz(std::cout, g);

	return 0;
}
