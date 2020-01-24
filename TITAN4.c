#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./setting.h"


struct beam{
  int stat;
  double px;
  double py;
  double angle;
};

struct region{
  int stat;   // 0=Air 1=Shield 2=Mirror 3=Detector
  double r, l, t, b;
  double angle;
};

int sim(struct region, struct region, struct region, struct region, struct region, struct region, struct region, struct region, struct region, struct beam *, FILE*);
void plot(struct region, struct region, struct region, struct region, struct region, struct region, struct region);
int histogram(void);

  
int main(void){

  if(MY * MA > 11.){
    printf("\t出力ファイルが1GB以上になりそうだよ\n");
  } else if(MY * MA > 60.){
    if(safety){
      printf("The output file will be over 10 GB.\n");
      printf("This setting is prohibited.\n");
      exit(1);
    }
  }

  double time = 5. * (double)MY * (double)MA + 25. * (double)MY * (double)MA / (double)MP / (double)MP;
  printf("\t%d秒ぐらいかかります\n", (int) floor(time));

  int count = 0;
  //double dx = 10.;

  FILE *fp;
  fp = fopen("data.dat", "w");
  FILE *gp;
  gp = fopen("result.dat", "w");


  // 部品の定義

  struct region det;
  det.stat = 3;
  det.l = x_max;
  det.r = x_max + dx;
  det.t = y_max;
  det.b = -100.;

  struct region mir1;
  mir1.stat = 2;
  mir1.l = mir1_x - siz_mir_x / 2.;
  mir1.r = mir1_x + siz_mir_x / 2.;
  mir1.t = mir1_y + siz_mir_y / 2.;
  mir1.b = mir1_y - siz_mir_y / 2.;
  mir1.angle = mir1_t;

  struct region mir2;
  mir2.stat = 2;
  mir2.l = mir2_x - siz_mir_x / 2.;
  mir2.r = mir2_x + siz_mir_x / 2.;
  mir2.t = mir2_y + siz_mir_y / 2.;
  mir2.b = mir2_y - siz_mir_y / 2.;
  mir2.angle = mir2_t;

  struct region sld1;
  sld1.stat = 1;
  sld1.l = sld1_l;
  sld1.r = sld1_l + siz_sld_x;
  sld1.t = sld1_t;
  sld1.b = y_min;

  struct region sld2;
  sld2.stat = 1;
  sld2.l = sld2_l;
  sld2.r = sld2_l + siz_sld_x;
  sld2.t = y_max;
  sld2.b = sld2_b;
  
  struct region sld3;
  sld3.stat = 1;
  sld3.l = sld3_l;
  sld3.r = sld3_l + siz_sld_x;
  sld3.t = sld3_t;
  sld3.b = y_min;

  struct region sld4;
  sld4.stat = 1;
  sld4.l = sld4_l;
  sld4.r = sld4_l + siz_sld_x;
  sld4.t = y_max;
  sld4.b = sld4_b;
  
  struct region sld5;
  sld5.stat = 1;
  sld5.l = 0.;
  sld5.r = x_max + dx;
  sld5.t = y_max + dx;
  sld5.b = y_max;

  struct region sld6;
  sld6.stat = 1;
  sld6.l = 0.;
  sld6.r = x_max + dx;
  sld6.t = y_min;
  sld6.b = y_min - dx;


  
  plot(det, mir1, mir2, sld1, sld2, sld3, sld4);

  printf("\tCくん演算中...\n");

  int iy, it;
  int iymax = (int) floor((mesitylen_t - mesitylen_b) / dy);
  int itmax = (int) floor((t_max - t_min) / dt);
  double y = mesitylen_b;
  double t;
  for(iy = 0; iy <= iymax; iy++){
    t = t_min;
    for(it = 0; it <= itmax; it++){

      // ビームの定義
      
      struct beam p1;
      p1.stat = 0;
      p1.px = 0.0;
      p1.py = y;
      p1.angle = t;

  
      // ビームシミュレーション
  
      count = 0;
      fprintf(fp, "%d\t%f\t%f\t%f\n", count, p1.px, p1.py, p1.angle);

      int i;
      int i_max = (int) floor(1.5*x_max/dx);
      for(i = 0; i < i_max; i++){
	count++;
	p1.px += dx * cos(p1.angle*pi/180.);
	p1.py += dx * sin(p1.angle*pi/180.);
	if(sim(det, mir1, mir2, sld1, sld2, sld3, sld4, sld5, sld6, &p1, gp)){
	  break;
	}
	fprintf(fp, "%d\t%f\t%f\t%f\n", count, p1.px, p1.py, p1.angle);
      }
      fprintf(fp, "\n");

      t += dt;
      
    }
    
    y += dy;
    
  }
  
  fclose(fp);
  fclose(gp);

  // Histogramを作る
  if(histogram()){
    printf("Failed to create histogram.\n");
  }
  
  // gnuplotで表示
  printf("\tGNUPLOTくん描画中...\n");
  FILE *gnuplot;
  gnuplot = popen("gnuplot -persist", "w");
  if(gnuplot == NULL){
    printf("Failed to open gnuplot\n");
    exit(1);
  }
  fprintf(gnuplot, "load \"plot.plt\"");
  pclose(gnuplot);

  printf("\tできたよ！\n");
  
  return 0;

}


int sim(struct region det, struct region mir1, struct region mir2, struct region sld1, struct region sld2, struct region sld3, struct region sld4, struct region sld5, struct region sld6, struct beam *particle, FILE *gp){

  double x = particle->px;
  double y = particle->py;
  int result = 0;
  
  // Detector
  if((x >= det.l) && (x <= det.r)){
    if((y >= det.b) && (y <= det.t)){
      if(particle->stat == 4){
	particle->stat = 0;
      } else if(particle->stat == 5){
	particle->stat = 3;
      } else if(particle->stat == 6){
	particle->stat = 0;
      }
      fprintf(gp, "%f\t%d\n", y, particle->stat);
      result = 1;
    }
  }

  // Mirror1
  if((x >= mir1.l) && (x <= mir1.r)){
    if((y >= mir1.b) && (y <= mir1.t)){

      // 1回だけ反射
      if(particle->stat == 0){

	// 適切な入射角だけ反射
	double angle = particle->angle - mir1.angle;
	if(((double)rand()/Rmax < tan(angle*pi/180.)*siz_mir_x*reflectivity) && (angle <= mir_t_c)){
	  //if((0.0 < tan(angle*pi/180.)/tan(pi/180.)) && (angle <= 1.)){
	  particle->stat = 1;
	  particle->angle -= 2.*angle;
	} else {
	  particle->stat = 4;
	}
      }
    }
  }

  // Mirror2
  if((x >= mir2.l) && (x <= mir2.r)){
    if((y >= mir2.b) && (y <= mir2.t)){

      // 1回だけ反射
      if(particle->stat == 1){

	// 適切な入射角だけ反射
	double angle = particle->angle - mir2.angle;
	if(((double)rand()/Rmax < tan(angle*pi/180.)*siz_mir_x*reflectivity) && (angle <= mir_t_c)){
	  particle->stat = 2;
	  particle->angle -= 2.*angle;
	}
      }

      // 1つ目をスルーしたやつ
      if((particle->stat == 0) || (particle->stat == 4)){

	// 適切な入射角だけ反射
	double angle = particle->angle - mir2.angle;
	if(((double)rand()/Rmax < tan(angle*pi/180.)*siz_mir_x*reflectivity) && (angle <= mir_t_c)){
	  particle->stat = 3;
	  particle->angle -= 2.*angle;
	} else {
	  particle->stat = 6;
	}
      }
    }
  }

  // Shield
  if((x >= sld1.l) && (x <= sld1.r)){
    if((y >= sld1.b) && (y <= sld1.t)){
      result = 1;
    }
  }
  if((x >= sld2.l) && (x <= sld2.r)){
    if((y >= sld2.b) && (y <= sld2.t)){
      result = 1;
    }
  }
  if((x >= sld3.l) && (x <= sld3.r)){
    if((y >= sld3.b) && (y <= sld3.t)){
      result = 1;
    }
  }
  if((x >= sld4.l) && (x <= sld4.r)){
    if((y >= sld4.b) && (y <= sld4.t)){
      result = 1;
    }
  }
  if((x >= sld5.l) && (x <= sld5.r)){
    if((y >= sld5.b) && (y <= sld5.t)){
      result = 1;
    }
  }
  if((x >= sld6.l) && (x <= sld6.r)){
    if((y >= sld6.b) && (y <= sld6.t)){
      result = 1;
    }
  }
  
  return result;

}


void plot(struct region det, struct region mir1, struct region mir2, struct region sld1, struct region sld2, struct region sld3, struct region sld4){

  FILE *fp;
  fp = fopen("plot.plt", "w");

  // Histogramの表示
  fprintf(fp, "set terminal %s\n", set_terminal);
  fprintf(fp, "set output \"%s\"\n", set_output);
  fprintf(fp, "set multiplot\n");
  fprintf(fp, "unset key\n");
  fprintf(fp, "set yrange [%f:%f]\n", y_min, y_max);
  fprintf(fp, "set lmargin screen 0.7\n");
  fprintf(fp, "set rmargin screen 0.95\n");
  fprintf(fp, "set tmargin screen 0.1\n");
  fprintf(fp, "set bmargin screen 0.95\n");
  fprintf(fp, "unset ylabel\n");
  fprintf(fp, "set format x \"\"\n");
  fprintf(fp, "set format y \"\"\n");
  fprintf(fp, "set style fill solid\n");
  fprintf(fp, "plot \"histogram.dat\" u (($2+$3+$4+$5)*0.5):($1):(($2+$3+$4+$5)*0.5):(0.5) with boxxyerrorbars lw 0.5 lc rgb \"light-pink\", ");
  fprintf(fp, "\"histogram.dat\" u (($2+$3+$5)*0.5):($1):(($2+$3+$5)*0.5):(0.5) with boxxyerrorbars lw 0.5 lc rgb \"light-blue\", ");
  fprintf(fp, "\"histogram.dat\" u (($2+$5)*0.5):($1):(($2+$5)*0.5):(0.5) with boxxyerrorbars lw 0.5 lc rgb \"light-green\", ");
  fprintf(fp, "\"histogram.dat\" u (($5)*0.5):($1):(($5)*0.5):(0.5) with boxxyerrorbars lw 0.5 lc rgb \"cyan\"\n");

  // 軌道表示の設定
  fprintf(fp, "set lmargin screen 0.1\n");
  fprintf(fp, "set rmargin screen 0.7\n");
  fprintf(fp, "set tmargin screen 0.1\n");
  fprintf(fp, "set bmargin screen 0.95\n");
  fprintf(fp, "set xrange [0:%f]\n", x_max);
  fprintf(fp, "set format x \"%%g\"\n");
  fprintf(fp, "set format y \"%%g\"\n");
  
  // Mirror1
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir1.l, mir1.t, mir1.l, mir1.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir1.l, mir1.t, mir1.r, mir1.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir1.r, mir1.t, mir1.r, mir1.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir1.l, mir1.b, mir1.r, mir1.b);

  // Mirror2
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir2.l, mir2.t, mir2.l, mir2.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir2.l, mir2.t, mir2.r, mir2.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir2.r, mir2.t, mir2.r, mir2.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc 2\n", mir2.l, mir2.b, mir2.r, mir2.b);

  // Shield1
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld1.l, sld1.t, sld1.l, sld1.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld1.l, sld1.t, sld1.r, sld1.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld1.r, sld1.t, sld1.r, sld1.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld1.l, sld1.b, sld1.r, sld1.b);
  
  // Shield2
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld2.l, sld2.t, sld2.l, sld2.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld2.l, sld2.t, sld2.r, sld2.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld2.r, sld2.t, sld2.r, sld2.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld2.l, sld2.b, sld2.r, sld2.b);
  
  // Shield3
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld3.l, sld3.t, sld3.l, sld3.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld3.l, sld3.t, sld3.r, sld3.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld3.r, sld3.t, sld3.r, sld3.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld3.l, sld3.b, sld3.r, sld3.b);
  
  // Shield4
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld4.l, sld4.t, sld4.l, sld4.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld4.l, sld4.t, sld4.r, sld4.t);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld4.r, sld4.t, sld4.r, sld4.b);
  fprintf(fp, "set arrow from %f,%f to %f,%f nohead lc rgb \"red\"\n", sld4.l, sld4.b, sld4.r, sld4.b);
  
  // Beam
  fprintf(fp, "plot \"data.dat\" u 2:3 every %d:%d w l lc rgb \"#%02xc000ff\"\n", MP, MP, clearness);

  // Histogram
  fprintf(fp, "unset multiplot\n");
  fprintf(fp, "set output\n");
  fprintf(fp, "set terminal %s\n", default_terminal);

  fclose(fp);
  
}



int histogram(void){

  FILE *res;
  res = fopen("result.dat", "r");
  if(res == NULL){
    printf("Failed to open \"result.dat\" \n");
    return 1;
  }

  FILE *fp;
  fp = fopen("histogram.dat", "w");
  fprintf(fp, "#y[mm]\tNoReflect\tOnlyMir1\tMir1&Mir2\tOnlyMir2\n");

  int i, j, n;
  int imax = 1e5;
  int n_max = (int) floor(y_max + 100.) + 1;
  int bin[4][n_max];
  for(i = 0; i < 4; i++){
    for(j = 0; j < n_max; j++){
      bin[i][j] = 0;
    }
  }
  double y;
  int stat;
  char textbuf[256];
  char *c;
  
  for(i = 0; i <= imax; i++){

    if(fgets(textbuf, 255, res) == NULL){
      break;
    }

    c = textbuf;
    y = strtod(c, &c);
    c++;
    stat = atoi(c);

    n = 100 + (int) floor(y);
    bin[stat][n]++;

  }

  for(n = 0; n < n_max; n++){
    fprintf(fp, "%f\t%d\t%d\t%d\t%d\n", (double)n -99.5, bin[0][n], bin[1][n], bin[2][n], bin[3][n]);
  }


  fclose(fp);
  fclose(res);
  return 0;

}
