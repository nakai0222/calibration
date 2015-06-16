#include "glutGraph.h"

int main(int argc, char* argv[]){
	// glutGraphを作成。引数の数で2Dのみ/3Dのみ/2D3D両方を選択できる。
	//    以下は640x480サイズ、100点のログを表示する2Dウィンドウと、
	//         512x512サイズの3Dウィンドウを生成する例。
	glutGraph graph(&argc, argv, 640, 480, 100, 512, 512);

	// 2Dのグラフを2つ追加
	//  　以下はRGB=(1.0,0.0,0.0)=赤のグラフを+0.5=上のほうに0.2倍の振幅で表示し、
	//    さｒにRGB=(0.0,1.0,0.0)=緑のグラフを-0.5=下のほうに0.3倍の振幅で表示する例。
	graph.pushGraph2D(1.0, 0.0, 0.0,  0.5, 0.2);
	graph.pushGraph2D(0.0, 1.0, 0.0, -0.5, 0.3);

	// 2Dで表示するサンプルデータの用意。
	//    以下では2つグラフがあるのでdata_2Dに2つの要素を追加している（初期値を0.0としている）。
	std::vector<double> data_2D;
	data_2D.push_back(0.0);
	data_2D.push_back(0.0);

	// 3Dの点群グループを2つ追加
	//    以下はRGB=(1.0,1.0,1.0)=白の点群グループを点で表示し、
	//    さらにRGB=(0.0,1.0,1.0)=シアンの点群グループを球で表示する例。
	graph.pushGraph3D(1.0, 1.0, 1.0, GL_POINTS);
	graph.pushGraph3D(0.0, 1.0, 1.0, GLUTGRAPH_SPHERE);
	
	// 3Dで表示するサンプルデータの用意。
	//    以下では2つ点群グループがあるのでdata_3Dに2つの要素を追加している。
	//    data_3D[0]は一点のみを持ち、data_3D[1]は円状に並んだ点を持つとしている。
	graphPoint3D tmp_3D;
	std::vector<graphPoint3D> group_3D;
	std::vector<std::vector<graphPoint3D>> data_3D;
	group_3D.push_back({0.0,0.0,0.0});
	data_3D.push_back(group_3D);
	group_3D.clear();
	for (int i = 0; i < 360; i++){
		tmp_3D.x = 30*cos(i/180.0*M_PI);
		tmp_3D.y = 30*sin(i/180.0*M_PI);
		tmp_3D.z = -10*cos(i / 180.0*M_PI);
		group_3D.push_back(tmp_3D);
	}
	data_3D.push_back(group_3D);

	graph.pushData3D(0, data_3D[0]);
	graph.pushData3D(1, data_3D[1]);

	double t = 0;
	while (1){
		// 2Dのグラフは全グラフを同時に更新する仕様。
		// 例ではグラフは2つ（要素番号[0],[1]に対応）なので、
		// data_2D[0],data_2D[1]を更新して表示。ログを超えたデータは流れていく。
		data_2D[0] = sin(  t/ 180 * M_PI);
		data_2D[1] = cos(2*t/ 180 * M_PI);
		graph.pushData2D(data_2D);

		// 3Dのグラフは点群グループ毎に更新する仕様。
		// 例では点群グループは2つ（第一引数0,1に対応）で、
		// data_3D[0]だけ更新（点を追加）して表示している。
		tmp_3D.x = 30 * sin(  t / 180 * M_PI);
		tmp_3D.y = 30 * sin(2.18*t / 180 * M_PI);
		tmp_3D.z = 30 * cos(3.49*t / 180 * M_PI);
		data_3D[0].push_back(tmp_3D);
		graph.pushData3D(0, data_3D[0]);

		t++;
	}
	return 0;
}
