#include <iostream>
#include <stack>
using namespace std;
class HermitePoly{
private:
	unsigned int n;
	double x;
public:
	HermitePoly(){
		n=0;
	}
	HermitePoly(unsigned int n,double x){
		this->n=n;
		this->x=x;
	}
	double calculate(){
		if(n==0)
			return 1;
		if(n==1)
			return 2*x;
		stack<double> currentHermite;
		currentHermite.push(1);
		currentHermite.push(2*x);
		for(int i=2;i<=n;i++){
			double previous1=currentHermite.top();
			currentHermite.pop();
			double previous2=currentHermite.top();
			currentHermite.pop();
			currentHermite.push(previous1);
			double newHermite=2*x*previous1-2*(i-1)*previous2;
			currentHermite.push(newHermite);
		}
		return currentHermite.top();
	}
	double calculateRec(int i){
		if(i==0)
			return 1;
		if(i==1)
			return 2*x;
		return 2*x*calculateRec(i-1)-2*(i-1)*calculateRec(i-2);
	}
	unsigned int getN(){
		return n;
	}
};
int main(){

	HermitePoly poly(80,2);
	cout<<poly.calculate();
	return 0;
}