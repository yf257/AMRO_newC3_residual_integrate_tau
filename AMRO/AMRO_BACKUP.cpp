#include "stdafx.h"
using namespace std;

//sdf
//int veloX(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);
//int veloY(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);
//int veloZ(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);

int veloX(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);
int veloY(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);
int veloZ(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out);

int fx(Ipp64f * field, Ipp64f *vy, Ipp64f *vz, int length, Ipp64f *temp, Ipp64f *out);
int fy(Ipp64f * field, Ipp64f *vx, Ipp64f *vz, int length, Ipp64f *temp, Ipp64f *out);
int fz(Ipp64f * field, Ipp64f *vx, Ipp64f *vy, int length, Ipp64f *temp, Ipp64f *out);

//int taufun(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, int length, Ipp64f *temp, Ipp64f *out);
int taufun(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, int length, Ipp64f *temp, Ipp64f *out, Ipp64f *one);
int _tmain(int argc, _TCHAR* argv[])
{
	//Ipp64f thetas[numthetas] = {0,3.1415926 };

	//Ipp64f thetas[1] = { 0 };
	//const int numthetas = 91;
	//Ipp64f thetas[91] = { 0., 0.0349066, 0.0698132, 0.10472, 0.139626, 0.174533, 0.20944, 0.244346, 0.279253, 0.314159, 0.349066, 0.383972, 0.418879, 0.453786, 0.488692, 0.523599, 0.558505, 0.593412, 0.628319, 0.663225, 0.698132, 0.733038, 0.767945, 0.802851, 0.837758, 0.872665, 0.907571, 0.942478, 0.977384, 1.01229, 1.0472, 1.0821, 1.11701, 1.15192, 1.18682, 1.22173, 1.25664, 1.29154, 1.32645, 1.36136, 1.39626, 1.43117, 1.46608, 1.50098, 1.53589, 1.5708, 1.6057, 1.64061, 1.67552, 1.71042, 1.74533, 1.78024, 1.81514, 1.85005, 1.88496, 1.91986, 1.95477, 1.98968, 2.02458, 2.05949, 2.0944, 2.1293, 2.16421, 2.19911, 2.23402, 2.26893, 2.30383, 2.33874, 2.37365, 2.40855, 2.44346, 2.47837, 2.51327, 2.54818, 2.58309, 2.61799, 2.6529, 2.68781, 2.72271, 2.75762, 2.79253, 2.82743, 2.86234, 2.89725, 2.93215, 2.96706, 3.00197, 3.03687, 3.07178, 3.10669, 3.14159 };
	//Ipp64f params[9] = { .056, 475, 525, -60, 16, 9, .5, 14, 8 };
	
	const int numthetas = 31;
	Ipp64f thetas[31] = { 0.,0.0581776,0.116355,0.174533,0.232711,0.290888,0.349066,0.407243,0.465421,0.523599,0.581776,0.639954,0.698132,0.756309,0.814487,0.872665,0.930842,0.98902,1.0472,1.10538,1.16355,1.22173,1.27991,1.33809,1.39626,1.45444,1.51262,1.5708,1.62897,1.68715,1.74533 };

	Ipp64f *condout = new Ipp64f[numthetas];
	//Ipp64f*condout = new Ipp64f[46];
	//Ipp64f tau = .5;
	//Ipp64f final = 10* params[0];//time final?
	
	Ipp64f field45 = 7.91209; // 45 tesla in appropriate units

	Ipp64f *params = new Ipp64f[10]; //last parameter is phi
	DataExtractor extractor("params.dat");
	params = extractor.getDataArray();
	Ipp64f final = 8 * params[1 - 1];
	long steps = 500;//number of time steps?
	Ipp64f h = final / steps;
	FindFermi Fermi("start.dat", params);
	


	int nPoints = Fermi.nPoints;
	cout << nPoints << endl;
	Ipp64f *starts = new Ipp64f[nPoints * 3];
	Fermi.ReturnStart(starts);
	Ipp64f *output = new Ipp64f[nPoints*steps * 3]; //stores evolution of orbit around Fermi surface
	Ipp64f *times = new Ipp64f[steps * nPoints]; //time steps

	std::clock_t startT;
	Ipp64f duration;

	Ipp64f *field = new Ipp64f[3];
	Ipp64f *vzStorage = new Ipp64f[steps*nPoints];
	Ipp64f *vz0Storage = new Ipp64f[nPoints];
	Ipp64f *DOS = new Ipp64f[nPoints];
	Ipp64f *ones = new Ipp64f[nPoints];//for inverting
	ippsSet_64f(1, ones, nPoints);
	Ipp64f *taus = new Ipp64f[nPoints];//phi dependent taus

	Ipp64f *vx = new Ipp64f[nPoints];
	Ipp64f *vy = new Ipp64f[nPoints];
	Ipp64f *vz = new Ipp64f[nPoints];

	Ipp64f *argx = new Ipp64f[nPoints];
	Ipp64f *argy = new Ipp64f[nPoints];
	Ipp64f *argz = new Ipp64f[nPoints];

	Ipp64f *tempx = new Ipp64f[16 * nPoints];
	Ipp64f *tempy = new Ipp64f[16 * nPoints];
	Ipp64f *tempz = new Ipp64f[16 * nPoints];



	Ipp64f *k1x = new Ipp64f[nPoints];
	Ipp64f *k1y = new Ipp64f[nPoints];
	Ipp64f *k1z = new Ipp64f[nPoints];
	Ipp64f *k2x = new Ipp64f[nPoints];
	Ipp64f *k2y = new Ipp64f[nPoints];
	Ipp64f *k2z = new Ipp64f[nPoints];
	Ipp64f *k3x = new Ipp64f[nPoints];
	Ipp64f *k3y = new Ipp64f[nPoints];
	Ipp64f *k3z = new Ipp64f[nPoints];
	Ipp64f *k4x = new Ipp64f[nPoints];
	Ipp64f *k4y = new Ipp64f[nPoints];
	Ipp64f *k4z = new Ipp64f[nPoints];//what are these? why do we need k1-4?

	Ipp64f total = 0;

	startT = std::clock();

	for (int j = 0; j < nPoints; j++) { //initialize Fermi surface to starting grid, only needs to be done once
		output[0 * nPoints + j] = starts[j * 3 + 0];
		output[1 * nPoints + j] = starts[j * 3 + 1];
		output[2 * nPoints + j] = starts[j * 3 + 2];
	}



	/*argx[0] = 1;
	argy[0] = 2;
	argz[0] = 1;
	veloZ(params, argx, argy, argz, 1, tempz, vz);
	cout << vz[0] << endl;
	*/

	for (int th = 0; th <numthetas; th++) {

		for (int p = 0; p < steps; p++) { //re-initialize times SLOW STEP CREATE TEMP VARIABLE
			//times[p] = -p;//why -p?
			ippsSet_64f(-p, &times[nPoints * p],nPoints);
		}
		ippsMulC_64f_I(h, times, steps*nPoints);//time stamps

		field[0] = field45 * sin(thetas[th]) * cos(params[9]);  //set field(phi == 0?)
		field[1] = field45 * sin(thetas[th]) * sin(params[9]);
		field[2] = field45 * cos(thetas[th]);
		//for (int z = 0; z < nPoints; z++) {
		//	if (th == 0 ) {
		//		cout << setprecision(20) << times[nPoints*2 + z] << " " << endl;
		//	}
		//}


		for (int i = 1; i < steps; i++) {

			ippsCopy_64f(&output[nPoints * (3 * (i - 1) + 0)], argx, nPoints);//copy arguments for k1;
			ippsCopy_64f(&output[nPoints * (3 * (i - 1) + 1)], argy, nPoints);
			ippsCopy_64f(&output[nPoints * (3 * (i - 1) + 2)], argz, nPoints);
			veloX(params, argx, argy, argz, nPoints, tempx, vx); //calculate velocities;
			veloY(params, argx, argy, argz, nPoints, tempy, vy);
			veloZ(params, argx, argy, argz, nPoints, tempz, vz);
			ippsCopy_64f(vz, &vzStorage[nPoints * (i - 1)], nPoints);//store vz for conductivity later
			
			taufun(params, argx, argy, nPoints, tempx, taus,ones);// calculate k dependent tau
			ippsDiv_64f_I(taus, &times[nPoints * (i-1)], nPoints);
			//ippsDivC_64f_I(tau, &times[nPoints * (i - 1)], nPoints);
			ippsExp_64f_I(&times[nPoints * (i-1)], nPoints);
			ippsMulC_64f_I((1E-12) * h, &times[nPoints * (i-1)], nPoints);
			
			//taufun(params, argx, argy, nPoints, tempx, taus);
			//for (int z = 0; z < nPoints; z++) {
			//	if (th == 0 && i == 30) {
			//		cout << setprecision(20) << taus[z] << " " << endl;
			//	}
			//}
			/*for (int z = 0; z < nPoints; z++) {
			if (th == 1 && i == 1) {
			cout << setprecision(20)<< argx[z] << " " << endl;
			}
			}*/


			fx(field, vy, vz, nPoints, tempx, k1x); //calculate evolution in k and store in k1
			fy(field, vx, vz, nPoints, tempy, k1y);
			fz(field, vx, vy, nPoints, tempz, k1z);

			ippsMulC_64f(k1x, h / 2, tempx, nPoints); //prep evolved k step for k2
			ippsMulC_64f(k1y, h / 2, tempy, nPoints);
			ippsMulC_64f(k1z, h / 2, tempz, nPoints);
			ippsAdd_64f(tempx, &output[nPoints * (3 * (i - 1) + 0)], argx, nPoints); //add step to previous k point, load into arguments for k2;
			ippsAdd_64f(tempy, &output[nPoints * (3 * (i - 1) + 1)], argy, nPoints);
			ippsAdd_64f(tempz, &output[nPoints * (3 * (i - 1) + 2)], argz, nPoints);
			veloX(params, argx, argy, argz, nPoints, tempx, vx); //calculate velocities;
			veloY(params, argx, argy, argz, nPoints, tempy, vy);
			veloZ(params, argx, argy, argz, nPoints, tempz, vz);
			fx(field, vy, vz, nPoints, tempx, k2x); //calculate evolution in k and store in k2
			fy(field, vx, vz, nPoints, tempy, k2y);
			fz(field, vx, vy, nPoints, tempz, k2z);

			ippsMulC_64f(k2x, h / 2, tempx, nPoints); //prep evolved k step for k3
			ippsMulC_64f(k2y, h / 2, tempy, nPoints);
			ippsMulC_64f(k2z, h / 2, tempz, nPoints);
			ippsAdd_64f(tempx, &output[nPoints * (3 * (i - 1) + 0)], argx, nPoints); //add step to previous k point, load into arguments for k3;
			ippsAdd_64f(tempy, &output[nPoints * (3 * (i - 1) + 1)], argy, nPoints);
			ippsAdd_64f(tempz, &output[nPoints * (3 * (i - 1) + 2)], argz, nPoints);
			veloX(params, argx, argy, argz, nPoints, tempx, vx); //calculate velocities;
			veloY(params, argx, argy, argz, nPoints, tempy, vy);
			veloZ(params, argx, argy, argz, nPoints, tempz, vz);
			fx(field, vy, vz, nPoints, tempx, k3x); //calculate evolution in k and store in k3
			fy(field, vx, vz, nPoints, tempy, k3y);
			fz(field, vx, vy, nPoints, tempz, k3z);

			ippsMulC_64f(k3x, h, tempx, nPoints); //prep evolved k step for k4
			ippsMulC_64f(k3y, h, tempy, nPoints);
			ippsMulC_64f(k3z, h, tempz, nPoints);
			ippsAdd_64f(tempx, &output[nPoints * (3 * (i - 1) + 0)], argx, nPoints); //add step to previous k point, load into arguments for k4;
			ippsAdd_64f(tempy, &output[nPoints * (3 * (i - 1) + 1)], argy, nPoints);
			ippsAdd_64f(tempz, &output[nPoints * (3 * (i - 1) + 2)], argz, nPoints);
			veloX(params, argx, argy, argz, nPoints, tempx, vx); //calculate velocities;
			veloY(params, argx, argy, argz, nPoints, tempy, vy);
			veloZ(params, argx, argy, argz, nPoints, tempz, vz);
			fx(field, vy, vz, nPoints, tempx, k4x); //calculate evolution in k and store in k4
			fy(field, vx, vz, nPoints, tempy, k4y);
			fz(field, vx, vy, nPoints, tempz, k4z);

			ippsMulC_64f_I(2, k2x, nPoints); //scale k2
			ippsMulC_64f_I(2, k2y, nPoints);
			ippsMulC_64f_I(2, k2z, nPoints);
			ippsMulC_64f_I(2, k3x, nPoints); //scale k3
			ippsMulC_64f_I(2, k3y, nPoints);
			ippsMulC_64f_I(2, k3z, nPoints);

			ippsAdd_64f(k1x, k2x, tempx, nPoints); //add k1 + k2 to temp
			ippsAdd_64f(k1y, k2y, tempy, nPoints);
			ippsAdd_64f(k1z, k2z, tempz, nPoints);

			ippsAdd_64f_I(k3x, tempx, nPoints); //add in k3
			ippsAdd_64f_I(k3y, tempy, nPoints);
			ippsAdd_64f_I(k3z, tempz, nPoints);

			ippsAdd_64f_I(k4x, tempx, nPoints); //add in k4
			ippsAdd_64f_I(k4y, tempy, nPoints);
			ippsAdd_64f_I(k4z, tempz, nPoints);

			ippsMulC_64f_I(h / 6, tempx, nPoints); //scale the entire sum
			ippsMulC_64f_I(h / 6, tempy, nPoints); //scale the entire sum
			ippsMulC_64f_I(h / 6, tempz, nPoints); //scale the entire sum

			ippsAdd_64f(&output[nPoints * (3 * (i - 1) + 0)], tempx, &output[nPoints * (3 * i + 0)], nPoints); //add sum to previous output and store
			ippsAdd_64f(&output[nPoints * (3 * (i - 1) + 1)], tempy, &output[nPoints * (3 * i + 1)], nPoints);
			ippsAdd_64f(&output[nPoints * (3 * (i - 1) + 2)], tempz, &output[nPoints * (3 * i + 2)], nPoints);
		}


		ippsCopy_64f(&output[nPoints * (3 * (steps - 1) + 0)], argx, nPoints);//get velocity for last point
		ippsCopy_64f(&output[nPoints * (3 * (steps - 1) + 1)], argy, nPoints);
		ippsCopy_64f(&output[nPoints * (3 * (steps - 1) + 2)], argz, nPoints);
		veloZ(params, argx, argy, argz, nPoints, tempz, vz);
		ippsCopy_64f(vz, &vzStorage[nPoints * (steps - 1)], nPoints);

		taufun(params, argx, argy, nPoints, tempx, taus, ones);// calculate k dependent tau for last point
		ippsDiv_64f_I(taus, &times[nPoints * (steps - 1)], nPoints);
		//ippsDivC_64f_I(tau, &times[nPoints * (steps - 1)], nPoints);
		ippsExp_64f_I(&times[nPoints * (steps - 1)], nPoints);
		ippsMulC_64f_I((1E-12)*h, &times[nPoints * (steps - 1)], nPoints);

		ippsCopy_64f(&output[nPoints * (0)], argx, nPoints);//initial velocities for DOS calc;
		ippsCopy_64f(&output[nPoints * (1)], argy, nPoints);
		ippsCopy_64f(&output[nPoints * (2)], argz, nPoints);
		veloX(params, argx, argy, argz, nPoints, tempx, vx); //velocities for DOS are stored in vx, vy, and vz buffers.
		veloY(params, argx, argy, argz, nPoints, tempy, vy);
		veloZ(params, argx, argy, argz, nPoints, tempz, vz);

		ippsSqr_64f_I(vx, nPoints);//in-place square of velocities
		ippsSqr_64f_I(vy, nPoints);
		ippsSqr_64f_I(vz, nPoints);

		ippsAdd_64f(vx, vy, tempx, nPoints);//add all square velocities
		ippsAdd_64f_I(vz, tempx, nPoints);
		ippsSqrt_64f_I(tempx, nPoints);//square root
		ippsDiv_64f(tempx, ones, DOS, nPoints);

	  //need to change!!
		//ippsDiv_64f_I(taus, times, steps);//exponential stuff, negative tau is taken care of in time
		//ippsExp_64f_I(times, steps);
		//ippsMulC_64f_I((1E-12)*h, times, steps);

		ippsCopy_64f(&vzStorage[0], vz0Storage, nPoints);//save initial velocity before exp

		for (int i = 0; i < steps; i++) {
			ippsMul_64f_I(&times[i * nPoints], &vzStorage[i * nPoints], nPoints); //multiply velocities by exp time factor
		}

		for (int i = 0; i < (steps - 1); i++) {
			ippsAdd_64f_I(&vzStorage[i*nPoints], &vzStorage[(i + 1)*nPoints], nPoints); //add all and accumulate in last vector
		}

		ippsMul_64f_I(DOS, &vzStorage[(steps - 1)*nPoints], nPoints);
		ippsMul_64f_I(vz0Storage, &vzStorage[(steps - 1)*nPoints], nPoints);//multiply by initial velocities

		ippsSum_64f(&vzStorage[(steps - 1)*nPoints], nPoints, &total);//sum all elements of velocity vector

		condout[th] = total;
	}



	duration = (std::clock() - startT) / (Ipp64f)CLOCKS_PER_SEC;
	cout << "time: " << duration << endl;

	ofstream fout;
	fout.open("conductivity.dat");
	fout.precision(20);

	for (int i = 0; i < numthetas; i++) {

		fout << thetas[i] << '\t' << condout[i] << endl;
		//fout <<  condout[i] << endl;
		//cout << thetas[i] << "\t" << condout[i] << endl;
	}

	fout.close();
	//fout.open("Fermitraject.dat");
	//fout.precision(15);
	//cout << nPoints << endl;
	//for (int i = 1; i <steps; i++) {

	//	fout << output[nPoints * (3 * (i) + 0)] <<'\t'<< output[nPoints * (3 * (i) + 1)] <<'\t'<< output[nPoints * (3 * (i) + 2)] << endl;
		//cout << thetas[i] << "\t" << condout[i] << endl;
	//}

	//fout.close();
	//for (int i = 0; i < steps; ++i) {
	//	cout << times[i] << " " << endl;
	//}
	cout << "end"<<endl;

	/*ofstream fout2;
	fout2.open("vztimes.dat");
	fout2.precision(15);

	for (int i = 0; i < steps; i++) {

	fout2 << times[i] << endl;
	}

	fout2.close();*/

	/*char a;
	cin >> a;


	delete[] output;


	while (true);*/
	return 0;
}

int veloX(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out)
{
	ippsMulC_64f(kx, 3.74767, temp, length); //term for sin(kx), param3
	vdSin(length, temp, &temp[1 * length]);
	ippsMulC_64f(&temp[1 * length], params[3 - 1] * 11.4215, out, length);

	ippsMulC_64f(kx, 3.74767, temp, length); //term for sin(kx)cos(ky), param4
	vdSin(length, temp, &temp[1 * length]);
	ippsMulC_64f(ky, 3.74767, temp, length);
	vdCos(length, temp, &temp[2 * length]);
	ippsMul_64f_I(&temp[1 * length], &temp[2 * length], length);
	ippsMulC_64f(&temp[2 * length], 22.8429*params[4 - 1], temp, length);
	ippsAdd_64f_I(temp, out, length);

	ippsMulC_64f(kx, 2 * 3.74767, temp, length); //term for sin(2 kx), param5
	vdSin(length, temp, &temp[1 * length]);
	ippsMulC_64f(&temp[1 * length], params[5 - 1] * 11.4215 * 2, temp, length);
	ippsAdd_64f_I(temp, out, length);

	ippsMulC_64f(kx, 3.74767, temp, length); //term for long complicated kz term, param6
	vdSin(length, temp, &temp[1 * length]); // sin kx
	vdCos(length, temp, &temp[2 * length]); // cos kx
	ippsMulC_64f(ky, 3.74767, temp, length);
	vdSin(length, temp, &temp[3 * length]); // sin ky
	vdCos(length, temp, &temp[4 * length]); // cos ky
	ippsMulC_64f(kx, 3.74767 / 2, temp, length);
	vdSin(length, temp, &temp[5 * length]); // sin kx/2
	vdCos(length, temp, &temp[6 * length]); // cos kx/2
	ippsMulC_64f(ky, 3.74767 / 2, temp, length);
	vdSin(length, temp, &temp[7 * length]); // sin ky/2
	vdCos(length, temp, &temp[8 * length]); // cos ky/2
	ippsMulC_64f(kz, 6.6, temp, length);//kz*c/2(c=13.2)
	vdCos(length, temp, &temp[9 * length]); // cos kz/2
	ippsMul_64f(&temp[6 * length], &temp[1 * length], &temp[10 * length], length);//cos kx/2 * sin kx
	ippsMulC_64f_I(65893 * 4, &temp[10 * length], length); // mult by constant
	ippsSub_64f(&temp[4 * length], &temp[2 * length], &temp[11 * length], length);//cos kx - cos ky
	ippsMulC_64f(&temp[11 * length], 65893, &temp[12 * length], length);//mult by constant
	ippsMul_64f(&temp[5 * length], &temp[12 * length], &temp[13 * length], length);// mult by sin kx/2
	ippsAdd_64f(&temp[13 * length], &temp[10 * length], temp, length);//add those two together
	ippsMul_64f_I(&temp[9 * length], temp, length);//mult by cos kz/2
	ippsMul_64f_I(&temp[11 * length], temp, length);//mult by cos kx - cos ky
	ippsMul_64f_I(&temp[8 * length], temp, length);//mult by cos ky/2
	ippsMulC_64f_I(params[6 - 1] * 0.0000866667, temp, length);
	//ippsMulC_64f_I(params[6 - 1], temp, length);
	ippsAdd_64f_I(temp, out, length);

	return 0;
}


int veloY(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out) {
	ippsMulC_64f(ky, 3.74767, temp, length); //term for sin(ky), param3
	vdSin(length, temp, &temp[1 * length]);
	ippsMulC_64f(&temp[1 * length], params[3 - 1] * 11.4215, out, length);

	ippsMulC_64f(kx, 3.74767, temp, length); //term for cos(kx)sin(ky), param4
	vdCos(length, temp, &temp[1 * length]);
	ippsMulC_64f(ky, 3.74767, temp, length);
	vdSin(length, temp, &temp[2 * length]);
	ippsMul_64f_I(&temp[1 * length], &temp[2 * length], length);
	ippsMulC_64f(&temp[2 * length], 22.8429*params[4 - 1], temp, length);
	ippsAdd_64f_I(temp, out, length);

	ippsMulC_64f(ky, 2 * 3.74767, temp, length); //term for sin(2 ky), param5
	vdSin(length, temp, &temp[1 * length]);
	ippsMulC_64f(&temp[1 * length], params[5 - 1] * 11.4215 * 2, temp, length);
	ippsAdd_64f_I(temp, out, length);

	ippsMulC_64f(kx, 3.74767, temp, length); //term for long complicated kz term, param6
	vdSin(length, temp, &temp[1 * length]); // sin kx
	vdCos(length, temp, &temp[2 * length]); // cos kx
	ippsMulC_64f(ky, 3.74767, temp, length);
	vdSin(length, temp, &temp[3 * length]); // sin ky
	vdCos(length, temp, &temp[4 * length]); // cos ky
	ippsMulC_64f(kx, 3.74767 / 2, temp, length);
	vdSin(length, temp, &temp[5 * length]); // sin kx/2
	vdCos(length, temp, &temp[6 * length]); // cos kx/2
	ippsMulC_64f(ky, 3.74767 / 2, temp, length);
	vdSin(length, temp, &temp[7 * length]); // sin ky/2
	vdCos(length, temp, &temp[8 * length]); // cos ky/2
	ippsMulC_64f(kz, 6.6, temp, length);//kz*c/2(c=13.2)
	vdCos(length, temp, &temp[9 * length]); // cos kz/2
	ippsMul_64f(&temp[8 * length], &temp[3 * length], &temp[10 * length], length);//cos ky/2 * sin ky
	ippsMulC_64f_I(65893 * 4, &temp[10 * length], length); // mult by constant
	ippsSub_64f(&temp[4 * length], &temp[2 * length], &temp[11 * length], length);//cos kx - cos ky CHECK SIGN
	ippsMulC_64f(&temp[11 * length], 65893, &temp[12 * length], length);//mult by constant
	ippsMul_64f(&temp[7 * length], &temp[12 * length], &temp[13 * length], length);// mult by sin kx/2
	ippsSub_64f(&temp[10 * length], &temp[13 * length], temp, length);//add those two together (neg sign)	
	ippsMul_64f_I(&temp[9 * length], temp, length);//mult by cos kz/2
	ippsMul_64f_I(&temp[11 * length], temp, length);//mult by cos kx - cos ky
	ippsMul_64f_I(&temp[6 * length], temp, length);//mult by cos kx/2	
	ippsMulC_64f_I(params[6 - 1] * 0.0000866667, temp, length);
	//ippsMulC_64f_I(params[6 - 1], temp, length);
	ippsAdd_64f_I(temp, out, length);
	return 0;
}

int veloZ(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out) {
	ippsMulC_64f(kx, 3.74767, temp, length); //term for long complicated kz term, param6
	vdCos(length, temp, &temp[2 * length]); // cos kx
	ippsMulC_64f(ky, 3.74767, temp, length);
	vdCos(length, temp, &temp[4 * length]); // cos ky
	ippsMulC_64f(kx, 3.74767 / 2, temp, length);
	vdCos(length, temp, &temp[6 * length]); // cos kx/2
	ippsMulC_64f(ky, 3.74767 / 2, temp, length);
	vdCos(length, temp, &temp[8 * length]); // cos ky/2
	ippsMulC_64f(kz, 6.6, temp, length);
	vdSin(length, temp, &temp[9 * length]); // sin kz/2c
	ippsMulC_64f(kz, 13.2, temp, length);
	vdSin(length, temp, &temp[13 * length]);// sin kzc

	ippsSub_64f(&temp[4 * length], &temp[2 * length], &temp[11 * length], length);//cos kx - cos ky
	ippsSqr_64f_I(&temp[11 * length], length);//square it
	ippsMul_64f_I(&temp[9 * length], &temp[11 * length], length);// times sin kz/2
	ippsMul_64f_I(&temp[8 * length], &temp[11 * length], length);// times cos ky/2
	ippsMul_64f_I(&temp[6 * length], &temp[11 * length], length);// times cos ky/2
	ippsMulC_64f(&temp[11 * length], params[6 - 1] * 10.0571, out, length);
	ippsMulC_64f(&temp[13 * length], params[7 - 1] * 10.0571 * 2, &temp[12 * length], length);//h7 term
	ippsAdd_64f_I(&temp[12 * length], out, length);


	return 0;
}



//int veloX(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out) {
//	/*return 5710 * sin(3.74767*kx);*/
//	ippsMulC_64f(kx, 3.74767, temp, length);
//	vdSin(length, temp, out);
//	ippsMulC_64f_I(5710, out, length);
//	return 0;
//}
//
//int veloY(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out) {
//	/*return 5710 * sin(3.74767*ky);*/
//	ippsMulC_64f(ky, 3.74767, temp, length);
//	vdSin(length, temp, out);
//	ippsMulC_64f_I(5710, out, length);
//	return 0;
//}
//
//int veloZ(Ipp64f *kx, Ipp64f *ky, Ipp64f *kz, int length, Ipp64f *temp, Ipp64f *out) {
//	/*return  .10 * sin(3.3*kz);*/
//	ippsMulC_64f(kz, 3.3, temp, length);
//	vdSin(length, temp, out);
//	ippsMulC_64f_I(0.1, out, length);
//	return 0;
//}

int fx(Ipp64f * field, Ipp64f *vy, Ipp64f *vz, int length, Ipp64f *temp, Ipp64f *out) {
	/*return -1 / (11538.5) * (vy * field[2] - vz*field[1]);*/
	ippsMulC_64f(vy, field[2], temp, length);
	ippsMulC_64f(vz, field[1], &temp[length], length);
	ippsMulC_64f_I(-1, &temp[length], length);
	ippsAdd_64f_I(&temp[length], temp, length);
	ippsMulC_64f(temp, 1 / (11538.5), out, length);//+1 to run back in time

	return 0;

}
int fy(Ipp64f * field, Ipp64f *vx, Ipp64f *vz, int length, Ipp64f *temp, Ipp64f *out) {
	//return -1 / (11538.5) * (vz * field[0] - vx*field[2]);
	ippsMulC_64f(vz, field[0], temp, length);
	ippsMulC_64f(vx, field[2], &temp[length], length);
	ippsMulC_64f_I(-1, &temp[length], length);
	ippsAdd_64f_I(&temp[length], temp, length);
	ippsMulC_64f(temp, 1 / (11538.5), out, length);//+1 to run back in time

	return 0;
}
int fz(Ipp64f * field, Ipp64f *vx, Ipp64f *vy, int length, Ipp64f *temp, Ipp64f *out) {
	//return -1 / (11538.5) * (vx * field[1] - vy*field[0]);
	ippsMulC_64f(vx, field[1], temp, length);
	ippsMulC_64f(vy, field[0], &temp[length], length);
	ippsMulC_64f_I(-1, &temp[length], length);
	ippsAdd_64f_I(&temp[length], temp, length);
	ippsMulC_64f(temp, 1 / (11538.5), out, length);//+1 to run back in time

	return 0;
}

//int taufun(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, int length, Ipp64f *temp, Ipp64f *out) {
//	ippsDiv_64f(kx, ky, temp, length);
//	ippsAtan_64f_A50(temp, &temp[length], length);
//	vdSin(length, &temp[length], &temp[2 * length]);
//	ippsSqr_64f_I(&temp[2 * length], length);
//	vdCos(length, &temp[length], &temp[3 * length]);
//	ippsSqr_64f_I(&temp[3 * length], length);
//	ippsSub_64f(&temp[3 * length], &temp[2 * length], &temp[4 * length], length);

//	ippsCopy_64f(&temp[4 * length], out, length);


//	return 0;
//}
int taufun(Ipp64f *params, Ipp64f *kx, Ipp64f *ky, int length, Ipp64f *temp, Ipp64f *out, Ipp64f *ones) {
	ippsDiv_64f(kx, ky, temp, length);
	//cout << temp[0] << endl;
	ippsAtan_64f_A50(temp, &temp[length], length);
	vdSin(length, &temp[length], &temp[2 * length]);//sin(arctan(ky/kx))
	ippsSqr_64f_I(&temp[2 * length], length);
	vdCos(length, &temp[length], &temp[3 * length]);//cos(arctan(ky/kx))
	ippsSqr_64f_I(&temp[3 * length], length);
	ippsSub_64f(&temp[2 * length], &temp[3 * length], &temp[4 * length], length);//sin(arctan(ky/kx))^2-cos(arctan(ky/kx))^2
	//cout << temp[2 * length] << endl;
	ippsPowx_64f_A50(&temp[4 * length], params[9-1], &temp[5 * length], length);
	//cout << temp[5 * length] << endl;
	ippsMulC_64f_I(params[8-1], &temp[5 * length],length);
	//cout << temp[5 * length] << endl;
	ippsAddC_64f_I(1/params[1-1], &temp[5 * length], length);
	//cout << temp[5 * length] << endl;
	
	//cout << temp[6 * length] << endl;
	
	
	ippsDiv_64f(&temp[5 * length], ones, out, length);
	//cout << temp[6 * length] << endl;
	

	


	return 0;
}