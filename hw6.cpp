#include <vector>
#include <math.h>
#include <boost/math/special_functions/factorials.hpp>

#include "gnuplot_iostream.h"

template<typename T>
void make_plot(std::vector<std::vector<T> > x_v_v, std::vector<std::vector<T> > y_v_v, std::vector<std::string> title_v,
               std::string x_label, std::string y_label,
               std::string plot_title, std::string out_url)
{
  // std::cout << "x_v_v= \n";
  // for (typename std::vector<std::vector<T> >::iterator it = x_v_v.begin(); it != x_v_v.end(); it++)
  //   std::cout << patch_all::vec_to_str<>(*it) << "\n";
  
  // std::cout << "y_v_v= \n";
  // for (typename std::vector<std::vector<T> >::iterator it = y_v_v.begin(); it != y_v_v.end(); it++)
  //   std::cout << patch_all::vec_to_str<>(*it) << "\n";
  // 
  std::string color_code_[] = {"#0060ad", "#DAA520", "#7F7F7F", "#D2691E", "#556B2F", "#DC143C", "#DA70D6", "#8B008B", "#1E90FF"};
  Gnuplot gp;
  
  std::vector<T> x_v;
  for (typename std::vector<std::vector<T> >::iterator it = x_v_v.begin(); it != x_v_v.end(); it++)
    x_v.insert(x_v.end(), it->begin(), it->end() );
  
  T min_x = *std::min_element(x_v.begin(), x_v.end() );
  T max_x = *std::max_element(x_v.begin(), x_v.end() );
  
  std::vector<T> y_v;
  for (typename std::vector<std::vector<T> >::iterator it = y_v_v.begin(); it != y_v_v.end(); it++)
    y_v.insert(y_v.end(), it->begin(), it->end() );
  
  T min_y = *std::min_element(y_v.begin(), y_v.end() );
  T max_y = *std::max_element(y_v.begin(), y_v.end() );
  
  if (out_url.compare("") != 0) {
    gp << "set term png size 800,600 enhanced font '/usr/share/fonts/dejavu/DejaVuSans.ttf' 12\n";
    // gp << "set term png enhanced font '/usr/share/fonts/dejavu/DejaVuSans.ttf' 12\n";
    gp << "set output \"" << out_url << "\"\n";
  }
  gp << "set key right top\n";
  gp << "set title '" << plot_title << "'\n";
  gp << "set xrange [" << min_x << ":" << max_x + 1 << "]\nset yrange [" << min_y << ":" << max_y + 1 << "]\n";
  gp << "set xlabel '" << x_label << "'\n";
  gp << "set ylabel '" << y_label << "'\n";
  gp << "set grid\n";
  
  
  for (int i = 0; i < x_v_v.size(); i++)
    gp << "set style line " << boost::lexical_cast<std::string>(i + 1) << " lc rgb '" << color_code_[i] << "' " << "pt " << boost::lexical_cast<std::string>(i + 1) << " lt 1 lw 2 ps 1\n";
    // gp << "set style line " << boost::lexical_cast<std::string>(i + 1) << " lc rgb '" << color_code_[i] << "' lt 1 lw 2 pt 5 ps 1.5\n";
  
  gp << "set logscale y\n";
  
  gp << "plot";
  for (int i = 0; i < x_v_v.size(); i++) {
    gp << "'-' u 1:2 title '" << title_v[i] << "' w linespoints ls " << boost::lexical_cast<std::string>(i + 1);
    if (i == x_v_v.size() - 1)
      gp << "\n";
    else
      gp << ", ";
  }
  
  typename std::vector<std::vector<T> >::iterator it_x_v, it_y_v;
  for (it_x_v = x_v_v.begin(), it_y_v = y_v_v.begin(); it_x_v != x_v_v.end(); it_x_v++, it_y_v++)
    gp.send1d(boost::make_tuple(*it_x_v, *it_y_v) );
}

int main()
{
  
  std::vector<std::vector<float> > x_v_v, y_v_v;
  std::vector<std::string> title_v;
  float c; // mu
  float ro_c, ro;
  float ro_c_ul = 3;
  float ro_c_inc = 0.02;
  // M/M/c/c with c = 25
  title_v.push_back("M/M/25/25");
  c = 25;
  std::vector<float> mmcc_25_x_v, mmcc_25_y_v;
  for (ro_c = ro_c_inc; ro_c < ro_c_ul; ro_c += ro_c_inc) {
    mmcc_25_x_v.push_back(ro_c);
    ro = ro_c*c;
    
    float denominator = 0;
    for (float i = 0; i <= c; i++)
      denominator += pow(ro, i) / boost::math::factorial<double>(i);
    mmcc_25_y_v.push_back(std::max(0.0001,
      (pow(ro, c) / boost::math::factorial<double>(c) ) / denominator) );
  }
  x_v_v.push_back(mmcc_25_x_v);
  y_v_v.push_back(mmcc_25_y_v);
  // M/M/1/c with mu = c = 1
  // title_v.push_back("M/M/1/1");
  // c = 1;
  // std::vector<float> mm1c_1_x_v, mm1c_1_y_v;
  // for (ro_c = ro_c_inc; ro_c < ro_c_ul; ro_c += ro_c_inc) {
  //   mm1c_1_x_v.push_back(ro_c);
  //   ro = ro_c*c;
    
  //   mm1c_1_y_v.push_back(std::max(0.0001,
  //     pow(ro, c)*(1 - ro) / (1 - pow(ro, c + 1) ) ) );
  // }
  // x_v_v.push_back(mm1c_1_x_v);
  // y_v_v.push_back(mm1c_1_y_v);
  // M/M/1/c with mu = c = 25
  title_v.push_back("M/M/1/25");
  c = 25;
  std::vector<float> mm1c_25_x_v, mm1c_25_y_v;
  for (ro_c = ro_c_inc; ro_c < ro_c_ul; ro_c += ro_c_inc) {
    mm1c_25_x_v.push_back(ro_c);
    ro = ro_c*c;
    
    mm1c_25_y_v.push_back(std::max(0.0001,
      pow(ro, c)*(1 - ro) / (1 - pow(ro, c + 1) ) ) );
  }
  x_v_v.push_back(mm1c_25_x_v);
  y_v_v.push_back(mm1c_25_y_v);
  // M/M/c/c with c = 50
  title_v.push_back("M/M/50/50");
  c = 50;
  std::vector<float> mmcc_50_x_v, mmcc_50_y_v;
  for (ro_c = ro_c_inc; ro_c < ro_c_ul; ro_c += ro_c_inc) {
    mmcc_50_x_v.push_back(ro_c);
    ro = ro_c*c;
    
    float denominator = 0;
    for (float i = 0; i <= c; i++)
      denominator += pow(ro, i) / boost::math::factorial<double>(i);
    mmcc_50_y_v.push_back(std::max(0.0001,
      (pow(ro, c) / boost::math::factorial<double>(c) ) / denominator) );
  }
  x_v_v.push_back(mmcc_50_x_v);
  y_v_v.push_back(mmcc_50_y_v);
  // M/M/1/c with mu = c = 50
  title_v.push_back("M/M/1/50");
  c = 50;
  std::vector<float> mm1c_50_x_v, mm1c_50_y_v;
  for (ro_c = ro_c_inc; ro_c < ro_c_ul; ro_c += ro_c_inc) {
    mm1c_50_x_v.push_back(ro_c);
    ro = ro_c*c;
    
    mm1c_50_y_v.push_back(std::max(0.0001,
      pow(ro, c)*(1 - ro) / (1 - pow(ro, c + 1) ) ) );
  }
  x_v_v.push_back(mm1c_50_x_v);
  y_v_v.push_back(mm1c_50_y_v);
  // 
  std::string out_url = "/cac/u01/mfa51/Desktop/comnet_hw/hw6.png";
  // std::string out_url = "";
  make_plot<float>(x_v_v, y_v_v, title_v,
                   "ro/c", "Blocking probability",
                   "", out_url);
  
  return 0;
}


