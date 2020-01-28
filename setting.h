/////////////////////////////////////////
// 手軽に精度を変える設定
/////////////////////////////////////////

// 増やすほどビームの数が増える
// 出発点の間隔の倍率
const double MY = 1.;
// 角度の間隔の倍率
const double MA = 1.;

// 描画の粗さ(大きいほど粗くなる)
// ヒストグラムには影響しない
const int MP = 1;

/////////////////////////////////////////




//////////////////////////////
// 全体の設定 
//////////////////////////////


// 出力ファイルが10GBを超える設定を拒否
// 0にすると強制実行
const int safety = 1; // 1

// xの最大値[mm] (detのx座標)
const double x_max = 5500.; // 5000.

// yの最小値[mm]
// -100から-50までの範囲
const double y_min = -100.; // -50.

// yの最大値[mm]
// 50以上
const double y_max = 200.; // 250.



//////////////////////////////
// ビームシミュレーションの仕様
//////////////////////////////


// 1ステップでビームが進む距離[mm]
const double dx = 10.; // 10.

// 減速材の上端y座標[mm]
const double mesitylen_t = 50.; // 50.

// 減速材の下端y座標[mm]
// これは触らない方がいい
const double mesitylen_b = -50.; // -50.

// ビーム出射点の間隔[mm]
const double dy = 0.5 * MY; // 0.5 * MY

// 出射ビーム角度の上限[degree]
const double t_max = 3.0; // 3.0

// 出射ビーム角度の下限[degree]
const double t_min = -3.0; // -3.0

// 出射ビームの角度の間隔[degree]
const double dt = 0.1 * MA; // 0.1 * MA
  



//////////////////////////////
// ミラーの仕様と配置
//////////////////////////////


// ミラーのx方向の長さ[mm]
const double siz_mir_x = 70.; // 70.

// ミラーのy方向の長さ[mm]
const double siz_mir_y = 50.; // 50.

// ミラーの臨界角[degree]
// 仕様1°なら少し大きめにとった方が良い
const double mir_t_c = 1.2; // 1.2

// ミラーの反射率
const double reflectivity = 0.8; // 0.8

// mir1の中心x座標[mm]
const double mir1_x = 2700.; // 2700.

// mir1の中心y座標[mm]
const double mir1_y = -30.; // 74.

// mir1の角度[degree]
// x軸に対する反射面の角度(反時計回り)
const double mir1_t = 1.; // 1.

// mir2の中心x座標[mm]
const double mir2_x = 4100.; // 3900.

// mir2の中心y座標[mm]
const double mir2_y = 20.; //74.

// mir2の角度[degree]
// x軸に対する反射面の角度
const double mir2_t = 3.; // -1.



//////////////////////////////
// シールドの仕様と配置
//////////////////////////////


// シールドのx方向の長さ[mm]
const double siz_sld_x = 100.; // 100.

// sld1は下固定
// sld1の左端x座標[mm]
const double sld1_l = 2000.; // 2000.

// sld1の上端y座標[mm]
const double sld1_t = -50.; // 30.

// sld2は上固定
// sld2の左端x座標[mm]
const double sld2_l = 2000.; // 2000.

// sld2の下端y座標[mm]
const double sld2_b = -10.; // 70.

// sld3は下固定
// sld3の左端x座標[mm]
const double sld3_l = 2500.; // 2500.

// sld3の上端y座標[mm]
const double sld3_t = -50.; // 52.

// sld4は上固定
// sld4の左端x座標[mm]
const double sld4_l = 2500.; // 2500.

// sld4の下端y座標[mm]
const double sld4_b = -10.; // 87.



//////////////////////////////
// GNUPLOT描画設定
//////////////////////////////


// 出力するターミナル
const char set_terminal[] = "aqua"; // "aqua"

// 出力先
const char set_output[] = "hogehoge"; 

// デフォルトのターミナル(環境依存)
const char default_terminal[] = "aqua"; // "aqua"

// ビームの透明度
// 0から255までの範囲
const int clearness = 0xd0; // 0xd0



//////////////////////////////
// 諸定数
//////////////////////////////

const double pi = M_PI;
const double Rmax = (double) RAND_MAX;
