#include "json.h"
#include "utils/Table.h"
#include "utils/TimeStat.h"
#include <gtest/gtest.h>

TEST(jsoncpp, speedtest)
	//__attribute__((no_warn_unused_result))
{
	/*
	 * Styled output vs. non-styled output
	 */
	Json::Value obj;
	{ // Load testdata.json into obj
		std::ifstream is("testdata.json");
		std::stringstream buf;
		buf << is.rdbuf();
		Json::Reader reader;
		bool ret = reader.parse(buf, obj);
		if (!ret) {
			fprintf(stderr,"Cannot parse testdata.json\n");
			exit(1);
		}
	}


	/*
	 * 1 = styled
	 * 2 = stream
	 */

	const int num = 1000; // number of trials
	TimeStat g1(num + 1);
	TimeStat g2(num + 1);
	{
		//std::string buf = obj.toStyledString();
		for (int i = 0; i < num; i++) {
			{
				static Json::StyledWriter w1;
				g1.tic();
				//buf = w1.write(obj);
				w1.write(obj);
				g1.toc();
			}
			{
				static Json::FastWriter w2;
				g2.tic();
				//buf = w2.write(obj);
				w2.write(obj);
				g2.toc();
			}

		}
	}

	const int m = 2;
	const int n = 3;
	const char* rows[m] = {"styled", "stream"};
	const char* cols[n] = {"min", "median", "max"};
	double data[m*n] = {
		g1.min(), g1.median(), g1.max(),
		g2.min(), g2.median(), g2.max()
	};

	Table table;
	table.dim(m, n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);

	char banner[BUFSIZ];
	snprintf(banner, sizeof(banner),
			"Json writing speed test, numtrials = %d\n"
			"Test data is the entire en-US highlights json file, which is 588K.\n"
			"Numbers are in number of CPU cycles."
			, num);
	table.print(banner);
}
