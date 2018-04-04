#include <cmath>
#include <limits>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

#include "Exception.hpp"
#include "Matrix.hpp"

#ifndef ANPI_QR_DECOMP_HPP
#define ANPI_QR_DECOMP_HPP

namespace anpi
{
	template<typename T>
	T sqr(T x){
		return x*x;
	}

	template<typename T>
	int sgn(T x){
		return (T(0) < x) - (x < T(0));
	}


	template<typename T>
	void qr(const anpi::Matrix<T>& A,
		anpi::Matrix<T>& Q,
		anpi::Matrix<T>& R){

		int n=A.rows();
		bool singular=false;
		R=A;
		std::vector<T> c(n);
		std::vector<T> d(n);
		T scale, sigma,sum,tau;

		for (int k=0;k<n-1;k++){
			scale=0.0;
			for(int i=k;i<n;i++) {
				scale=std::max(scale,std::abs(R(i,k)));
			}
			if (scale==0.0){
				singular=true;
				c[k]=d[k]=0.0;	
				throw anpi::Exception("Intentando resolver sistema singular");
			}else {
				for (int i=k;i<n;i++){
					R(i,k) = R(i,k)/ scale;
				}
				sum=0.0;
				for (int i=k;i<n;i++){
					sum=sum+ sqr(R(i,k));

				}
				sigma= sgn(R(k,k))*sqrt(sum);
				R(k,k)= R(k,k)+sigma;
				c[k]=sigma*R(k,k);
				d[k]= (-scale*sigma);
				for(int j=k+1;j<n;j++){
					sum=0.0;
					for(int i=k;i<n;i++){
						sum = sum + R(i,k)*R(i,j);
					} 
					tau=sum/c[k];
					for(int i=k;i<n;i++){
						R(i,j)= R(i,j)-tau*R(i,k);
					}
				}
			}
		}

		d[n-1]=R(n-1,n-1);
		if (d[n-1]==0.0){
			singular=true;
			throw anpi::Exception("Intentando resolver sistema singular");
		}
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				Q(i,j)=0.0;
			}
			Q(i,i)=1.0;
		}

		for(int k=0;k<n-1;k++){
			if (c[k]!=0.0){
				for(int j=0;j<n;j++){
					sum=0.0;
					for(int i=k;i<n;i++){
						sum=sum+R(i,k)*Q(i,j);
					}
					sum=sum/c[k];
					for(int i=k;i<n;i++){
						Q(i,j)=Q(i,j)-sum*R(i,k);
					}
				}
			}
		}
		for(int i =0;i<n;i++){
			R(i,i)=d[i];
			for(int j=0;j<i;j++){
				R(i,j)=0.0;
			}
		}
		//std::cout<<"singular: "<< singular<< std::endl;
		
	}


	template<typename T>
	bool solveQR(const anpi::Matrix<T>& A,
		std::vector<T>& x,
		const std::vector<T>& b){
 	 	std::cout<<" entra a solveQR "<<std::endl;

 	 	anpi::Matrix<float> Q=anpi::Matrix<float>(A.rows(),A.cols(),0.0);
  		anpi::Matrix<float> R=anpi::Matrix<float>(A.rows(),A.cols(),0.0);
		anpi::qr(A,Q,R);
		

		std::cout << "A" << std::endl;
		printM(A);
		std::cout << "Q" << std::endl;
		printM(Q);
		std::cout << "R" << std::endl;
		printM(R);


		qtMult(b,x,Q);
		rSolve(b,x,R);

	}


	template<typename T>
	void qtMult(const std::vector<T>& b,std::vector<T>& x,anpi::Matrix<T>& Q){
		std::cout<<" entra a qtMult "<<std::endl;
		T sum;
		int n=Q.rows();
		for (int i=0;i<n;i++){
			sum=0.0;
			for (int j=0;j<n;j++){
				sum= sum+ Q(i,j)*b[j];
				x[i]=sum;
				std::cout << "x[i]" << x[i] << std::endl;
			}
		}
	}

	template<typename T>
	void rSolve(const std::vector<T>& b,std::vector<T>& x,anpi::Matrix<T>& R ){
		std::cout<<" entra a rSolve "<<std::endl;
		T sum;
		int n=R.rows();
		for (int i=n-1;i>=0;i--){
			sum=b[i];
			for(int j=i+1;j<n;j++){
				sum = sum- R(i,j)*x[j];
				std::cout << "sum: " << sum << std::endl;
			}
			std::cout << "R(i,i)" << R(i,i) << std::endl;
			std::cout << "Xi: ";
			std::cout << x[i] << std::endl;
			x[i]=sum/R(i,i);
			
		}
	}






	
}
#endif