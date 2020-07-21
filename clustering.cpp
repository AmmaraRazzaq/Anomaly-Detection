#include <iostream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;

struct cluster {
	vector<double> nodeID; //id of the node to which this cluster belongs
	double centroid;
	vector<double> points;
};

struct Sensor_node {
	double node_id;
	vector<double> node_values;
	double LLS;
	double LLSS;
	double LN;
	double xmax;
	double xmin;
	double NG;
	double Gmean, Gvariance;
	double Gmax, Gmin;
	vector<double> Cvalues; //values after data conditioning
	vector<cluster> Clusters;
	vector<cluster> CombinedC; //combined clusters
	vector<double> CombinedV; //combined values
};
struct topology {
	Sensor_node node1;  //child node 1
	Sensor_node node2;  //child node 2
	Sensor_node Gnode;  //gateway node
};

double Distance(double a, double b);
vector<cluster> merge(vector<cluster> V, double width);
vector<cluster> merge1(vector<cluster> V, double width);
void print (vector<cluster> P);
void print1 (vector<cluster> P);
void anomaly_detection(vector<cluster> V,int Approach);
vector<cluster> clustering(vector<double> sensor_data, double width);
vector<double> enter_values();
double Sum(vector<double> values);
double sum_of_squares(vector<double> values);
double max_value(vector<double> values);
double min_value(vector<double> values);
double Max(double a, double b, double c);
double Min(double a, double b, double c);

int main()
{
	cout<<"This algorithm is designed for one level hierarchy topology with one gateway node and two children nodes\n";
	topology T;
	vector<Sensor_node> node;
	/*cout<<"enter values for child node 1: \n";
	T.node1.node_values = enter_values();
	cout<<"enter values for child node 2: \n";
	T.node2.node_values = enter_values();
	cout<<"enter values for gateway node: \n";
	T.Gnode.node_values = enter_values();*/
	for(int i=0; i<3; i++)
	{
		if(i==0 || i==1)
			cout<<"enter the values of child node["<<i<<"]\n";
		else
			cout<<"enter the values of gateway node\n";

		//node[i].node_values = enter_values(); //edit it later //we will need a mechanism to push one vector to the other vector
		node.push_back(Sensor_node());
		cout<<"enter the values: 0 to quit\n";
		bool more=true;
		while(more)
		{
			double v;
			cin>>v;
			if(v == 0)
				more = false;
			else
			//values.push_back(v);
			node[i].node_values.push_back(v);
		}
			node[i].node_id = i;
	}


	int approach;  //cantralized or distributed?
	cout<<"enter the approach, 1 for centralized or 2 for distributed\n";
	cin>>approach;
	if(approach == 1) //centralized case
	{
		////////////combine all nodes values at gateway node//////////////////////////////
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<node[i].node_values.size(); j++)
			{
				//double s = T.Gnode.CombinedV.size();
				//cout<<"s"<<s<<"\n";
				//T.Gnode.CombinedC.push_back(cluster());

					double x = node[i].node_values[j];
					T.Gnode.CombinedV.push_back(x);

				//T.Gnode.CombinedC[s].centroid = node[i].Clusters[j].centroid;
			}
		}

		//////////////perform clustering on combined data values////////////////////////////
		////////////cluster the sensor data//////////////////////////
		double w; //width of cluster specified by user
		cout<<"enter the required width of cluster: ";
		cin>>w;
		vector<cluster> clusters = clustering(T.Gnode.CombinedV, w);

		print(clusters);     //print clusters

		////////clusters merging function ////////////////////////
		vector<cluster> M;
		double width = 7;
		M = merge(clusters,width);
		cout<<"CLUSTERS AFTER MERGING ARE: \n";
		print(M);
		/////////clustering anomaly detection////////////////////////
		anomaly_detection(M , approach); //anomaly detection is performed on merged clusters


	}
	else //distributed case
		if(approach == 2)
		{

		/*T.node1.LLS = sum(T.node1.node_values);
		T.node2.LLS = sum(T.node2.node_values);
		T.Gnode.LLS = sum(T.Gnode.node_values);

		T.node1.LLSS = sum_of_squares(T.node1.node_values);
		T.node2.LLSS = sum_of_squares(T.node2.node_values);
		T.Gnode.LLSS = sum_of_squares(T.Gnode.node_values);

		T.node1.LN = T.node1.node_values.size();
		T.node2.LN = T.node2.node_values.size();
		T.Gnode.LN = T.Gnode.node_values.size();

		T.node1.xmax = max_value(T.node1.node_values);
		T.node2.xmax = max_value(T.node2.node_values);
		T.Gnode.xmax = max_value(T.Gnode.node_values);

		T.node1.xmin = min_value(T.node1.node_values);
		T.node2.xmin = min_value(T.node2.node_values);
		T.Gnode.xmin = min_value(T.Gnode.node_values);*/
		//for(int i=0; i<node.size(); i++)  //calculate all local conditioning parameters
		//{
		//	node[i].LLS = Sum(node[i].node_values);
		//	node[i].LLSS = sum_of_squares(node[i].node_values);
		//	node[i].LN = node[i].node_values.size();
		//	node[i].xmax = max_value(node[i].node_values);
		//	node[i].xmin = min_value(node[i].node_values);
		//}

		//int i=0;
		//{
		//	T.node1.node_id = node[i].node_id;
		//	T.node2.node_id = node[i+1].node_id;
		//	T.Gnode.node_id = node[i+2].node_id;
		//
		//	T.Gnode.Gmax = Max(node[i].xmax,node[i+1].xmax,node[i+2].xmax); //calculate all global conditioning parameters
		//	T.Gnode.Gmin = Min(node[i].xmin,node[i+1].xmin,node[i+2].xmin);
		//	T.Gnode.NG = node[i].LN + node[i+1].LN + node[i+2].LN;
		//	T.Gnode.Gmean = (node[i].LLS + node[i+1].LLS + node[i+2].LLS)/T.Gnode.NG;
		//	double variance=0;
		//	for(int i=0; i<3; i++)
		//	{
		//		variance = variance + (node[i].LLSS - pow(T.Gnode.Gmean,2));
		//	}
		//	variance = variance/T.Gnode.NG;
		//	T.Gnode.Gvariance = sqrt(variance);
		//}

		//for(int i=0; i<3; i++)   //global parameters are updated at every node, each node will use these to condition its local data
		//{
		//	node[i].Gmax = T.Gnode.Gmax;
		//	node[i].Gmin = T.Gnode.Gmin;
		//	node[i].Gmean = T.Gnode.Gmean;
		//	node[i].Gvariance = T.Gnode.Gvariance;
		//}

		//for(int i=0; i<3; i++) //perform data conditioning on sensor values
		//{
		//	for(int j=0; j<node[i].node_values.size(); j++)
		//	{
		//		double x = (node[i].node_values[j] - node[i].Gmean)/node[i].Gvariance;
		//		x = (x - node[i].Gmin)/(node[i].Gmax - node[i].Gmin);
		//		node[i].Cvalues.push_back(x);
		//	}
		//}

		/////////////////print node values after data conditioning ////////////////////////////////////////
		//cout<<"Values after data conditioning are\n";
		//for(int i=0; i<node.size(); i++)
		//{
		//	cout<<"\nNODE ["<<i<<"]: (";
		//	for(int j=0; j<node[i].Cvalues.size(); j++)
		//	{
		//		cout<<node[i].Cvalues[j]<<" , ";
		//	}
		//}


		///////////perform the clustering at every node//////////////////////////////////////////////
		//double n;
		//cout<<"enter number of times you want to enter the width\n";
		//cin>>n;
		//for(int i=0; i<n; i++)
		//{
		double w; //width of cluster specified by user
		cout<<"enter the required width of cluster: ";
		cin>>w;
		for(int i=0; i<3; i++)
		{
			node[i].Clusters = clustering(node[i].node_values, w); //clustering algorithm
			for(int j=0; j<node[i].Clusters.size(); j++)
			{
				node[i].Clusters[j].nodeID.push_back(i);  //now every cluster contains an id of the node to which it belongs
			}

			print(node[i].Clusters);
		}

		////////////////combine the clusters at parent node/////brute force method///////////////


		//double s = T.Gnode.CombinedC.size();
		//double s=0;
		for(int i=0; i<3; i++)
		{

			for(int j=0; j<node[i].Clusters.size(); j++)
			{

				double s = T.Gnode.CombinedC.size();

				cout<<"s"<<s<<"\n";
				T.Gnode.CombinedC.push_back(cluster());
				for(int k=0; k<node[i].Clusters[j].points.size(); k++)
				{

					double x = node[i].Clusters[j].points[k];
					T.Gnode.CombinedC[s].points.push_back(x);
				}
				T.Gnode.CombinedC[s].centroid = node[i].Clusters[j].centroid;
				T.Gnode.CombinedC[s].nodeID.push_back(i); //new


			}

		}
		cout<<"combined set of clusters is:\n";
		print(T.Gnode.CombinedC);
		 //////////clusters merging function ////////////////////////
		vector<cluster> M;
		//double width = 7;
		M = merge1(T.Gnode.CombinedC,w);
		cout<<"CLUSTERS AFTER MERGING ARE: \n";
		print(M);
	///////////clustering anomaly detection////////////////////////
		anomaly_detection(M, approach); //anomaly detection is performed on merged clusters

		/*for(int i=0; i<node[i].Clusters.size(); i++)
		{
			node[i].Clusters.pop_back();
		}*/


		/*for(int i=0; i<T.Gnode.CombinedC.size(); i++)
		{
			for(int j=0; j<T.Gnode.CombinedC[i].points.size(); j++)
			{
				T.Gnode.CombinedC[i].points.pop_back();
			}

			for(int k=0; k<T.Gnode.CombinedC[i].nodeID.size(); k++)
			{
				T.Gnode.CombinedC[i].nodeID.pop_back();
			}

			T.Gnode.CombinedC.pop_back();
		}*/

		//} // end of for for the number of times we enter the width


	} //if

		else
		{
			//nothing
		}









	////////////cluster the sensor data//////////////////////////
	//double w; //width of cluster specified by user
	//cout<<"enter the required width of cluster: ";
	//cin>>w;
	//vector<cluster> clusters = clustering(values, w);

	// print(clusters);     //print clusters


    //////////clusters merging function ////////////////////////
	//vector<cluster> M;
	//double width = 7;
	//M = merge(clusters,width);
	//cout<<"CLUSTERS AFTER MERGING ARE: \n";
	//print(M);
	///////////clustering anomaly detection////////////////////////
	//anomaly_detection(M); //anomaly detection is performed on merged clusters

	getchar();
	//cout<<"hello i am your term project :P \n";
	getchar();
	return 0;
	getchar();
}

double Distance(double a, double b)
{
	double d;
	d = abs((b-a));
	return d;
}

vector<cluster> clustering(vector<double> sensor_data, double width)
{
	vector<cluster> C; //vector of clusters
	C.push_back(cluster());
	C[0].centroid = sensor_data[0];
	C[0].points.push_back(sensor_data[0]);

	for(int i=1; i<sensor_data.size(); i++)   // check the distance between 2nd data point and the centroids of every cluster.
	{
        int count = 0;                              // enter the data point to the cluster or create a new cluster based on the conditions
		for(int j=0; j<C.size();j++)
		{
			double d = Distance(sensor_data[i],C[j].centroid);

			if(d <= width)                         // enter the data point to the cluster
			{
				count = count + 1;
				//C.push_back(cluster());
				//double s = C[j].points.size();
				//C[j].points[s] = values[i];
				C[j].points.push_back(sensor_data[i]);
				double sum=0;
				for(int k=0; k<C[j].points.size(); k++)
				{
					sum = sum + C[j].points[k];
				}

				C[j].centroid = sum/C[j].points.size();
			}
			else                            // create a new cluster
			{

				// nothing
			}
		}     // second for loop is closed here.

		if(count == 0)              //create a new cluster
		{
		int q = C.size();
				C.push_back(cluster());
				C[q].centroid = sensor_data[i];
				C[q].points.push_back(sensor_data[i]);
		}
	}  // first for loop is closed here

	return C;
}


	// now that we have clusters, we have to merge them if any of them can be merged
vector<cluster> merge(vector<cluster> V, double width)
{
		int count;
		for(int i=0; i<V.size(); i++)
		{
			count=0;
			for(int j=0; j<V.size()&&(count==0); j++)
			{
				if(i != j )
				{
					double d = Distance(V[i].centroid,V[j].centroid);
					if(d < width)   // merge V[i] and V[j]  // create a new cluster
					{
							int q = V.size();
				            V.push_back(cluster());
							int n1 = V[i].points.size();
							int n2 = V[j].points.size();

							V[q].centroid = (n1*V[i].centroid + n2*V[j].centroid)/(n1+n2);
							//int n = V[i].points.size() + V[j].points.size();
							for(int k=0; k<V[i].points.size(); k++)
							{
								V[q].points.push_back(V[i].points[k]);
							}

							for(int k=0; k<V[j].points.size(); k++)
							{
								V[q].points.push_back(V[j].points[k]);
							}

							// now that a new cluster is created by merging V[i] and V[j]
							// delete V[i] and V[j]

							int last_pos = V.size() - 1;
							V[i] = V[last_pos];
							V.pop_back();

							int last_position = V.size() - 1;
							V[j] = V[last_position];
							V.pop_back();
							count = 1;
					}
				}
			}


		}

		return V;

}

void print (vector<cluster> P)
{
	cout<<"number of clusters are: "<<P.size()<<"\n";    //print clusters
	for(int i=0; i<P.size(); i++)
	{

		cout<<"Cluster["<<i<<"]\n";
		cout<<"data points of cluster: ";
		double s = P[i].points.size();
		for(int j=0; j<s; j++)
		{
		cout<<P[i].points[j]<<"\t";
		}

		cout<<"\n centroid of cluster is: "<<P[i].centroid<<"\n";
	}
}

/////////////////////Anomaly Detection Algorithm///////////////////////////////////
void anomaly_detection(vector<cluster> V, int Approach)
{
	vector<double> ICDi;  //average inter cluster distance for each cluster ci
	vector<double> Dci;   //set of inter-cluster distances d(ci,cj):Euclidean distance between centroids of ci & cj
	double ICD; //ICD value of single cluster
	double K = 5; //specified by user
	//cout<<"specify the value of K for K Nearest Neighbors: ";
	//cin>>K;
	///////STEP#1: compute Dci for each cluster ci////////////////
	for(int i=0; i<V.size(); i++)
	{
		//cout<<"for iteration: "<<i<<"\n";
		for(int j=0; j<V.size(); j++) //d(ci,cj) such that i!=j e.g, c1 will be compared to every cj such that j!=1
		{
			if(i != j)
			{
				double d = Distance(V[i].centroid,V[j].centroid); //distance between centroids of ci and cj
				Dci.push_back(d);
			}

		}

		//cout<<"for cluster["<<i<<"] the set of intercluster distances is: \n";
		/*for(int i=0; i<Dci.size(); i++)
		{
			cout<<Dci[i]<<"\n";
		}*/

		if (K < (V.size()-1))
		{
		double sum = 0;
		int index=0;
		for (int i=0; i<K; i++)
		{
			double smallest = Dci[0];
			for(int j=1; j<Dci.size(); j++)
			{
				if (Dci[j]<smallest)
				{
					index = j;
					smallest = Dci[j];
				}
			}

			sum = sum + smallest;
			int last_pos = Dci.size()-1;
			Dci[index] = Dci[last_pos];
			Dci.pop_back();
		} //end of for

		//cout<<"sum of K smallest distances is : "<<sum<<"\n";

		ICD = sum/K;
		//cout<<"value of ICD is"<<ICD<<"\n";
		ICDi.push_back(ICD);

		} // end of if
		else
			if(K >= (V.size()-1))
			{
				double sum=0;
				for(int i=0; i<Dci.size(); i++)
				{
					sum = sum+Dci[i];
				}

				ICD = sum/Dci.size();
				ICDi.push_back(ICD);
			}
		for(int i=0; i<Dci.size(); i++)
			Dci.pop_back();
	} //end of for loop of step 1

	//STEP#2 find AVG(ICD) and SD(ICD)

	double avg_ICD;
	double summ=0;
	for(int i=0; i<ICDi.size(); i++)
	{
		summ = summ + ICDi[i];
	}
	avg_ICD = summ/ICDi.size();
	//cout<<"the value of AVG(ICD) is: "<<avg_ICD<<"\n";

	double SD_ICD;
	double summation=0;
	for(int i=0; i<ICDi.size(); i++)
	{
		summation = summation + pow((ICDi[i] - avg_ICD),2);
		//cout<<"individual square value: "<<summation<<"\n";
	}

	summation = summation/(ICDi.size()-1);
	//cout<<"before square root"<<summation<<"\n";
	SD_ICD = sqrt(summation);
	//cout<<"the value of SD(ICD) is: "<<SD_ICD<<"\n";

	//STEP#3 find anomalous cluster
	for(int i=0; i<ICDi.size(); i++)
	{
		if(ICDi[i]> avg_ICD + SD_ICD)
		{
			cout<<"\n cluster["<<i<<"] is anomalous\n";
			if(Approach == 2)
			{
				cout<<"This cluster belongs to nodes: ";
				for(int j=0; j<V[i].nodeID.size(); j++)
				{
					cout<<V[i].nodeID[j]<<" ";
				}
			}
			else
			{
				//nothing
			}
		}
	}

	for(int i=0; i<ICDi.size(); i++)
	{
		ICDi.pop_back();
	}

}

vector<double> enter_values()
{
	vector<double> values;

	if(values.size() != 0)
	{
		for(int i=0; i<values.size(); i++)
		{
			values.pop_back();
		}
	}
	cout<<"enter the values: 0 to quit\n";
	bool more=true;
	while(more)
	{
		double v;
		cin>>v;
		if(v == 0)
			more = false;
		else
		values.push_back(v);
	}

	return values;

}

double Sum(vector<double> values)
{
	double sum=0;
	for(int i=0; i<values.size(); i++)
	{
		sum = sum + values[i];
	}

	return sum;
}

double sum_of_squares(vector<double> values)
{
	double result=0;
	double x;
	for(int i=0; i<values.size();i++)
	{
		x = pow(values[i],2);
		result = result + x;
	}

	return result;
}

double max_value(vector<double> values)
{
	double max = values[0];
	for(int i=0; i<values.size(); i++)
	{
		if(values[i] > max)
			max = values[i];
	}

	return max;
}

double min_value(vector<double> values)
{
	double min = values[0];
	for(int i=0; i<values.size(); i++)
	{
		if(values[i] < min)
			min = values[i];
	}

	return min;
}
double Max(double a, double b, double c)
{
	double max;
	if(a>b)
		max=a;
	else
		max=b;
	if(max < c)
		max=c;
	else
	{
	}

	return max;
}

double Min(double a, double b, double c)
{
	double min;
	if(a<b)
		min=a;
	else
		min=b;
	if(min > c)
		min=c;
	else
	{
	}

	return min;
}

vector<cluster> merge1(vector<cluster> V, double width) //for ditributed case
{
		int count;
		for(int i=0; i<V.size(); i++)
		{
			count=0;
			for(int j=0; j<V.size()&&(count==0); j++)
			{
				if(i != j )
				{
					double d = Distance(V[i].centroid,V[j].centroid);
					if(d < width)   // merge V[i] and V[j]  // create a new cluster
					{
							int q = V.size();
				            V.push_back(cluster());
							int n1 = V[i].points.size();
							int n2 = V[j].points.size();

							V[q].centroid = (n1*V[i].centroid + n2*V[j].centroid)/(n1+n2);
							for(int k=0; k<V[i].nodeID.size(); k++)
							{
								V[q].nodeID.push_back(V[i].nodeID[k]);
							}
							for(int k=0; k<V[j].nodeID.size(); k++)
							{
								V[q].nodeID.push_back(V[j].nodeID[k]);
							}
							//int n = V[i].points.size() + V[j].points.size();
							for(int k=0; k<V[i].points.size(); k++)
							{
								V[q].points.push_back(V[i].points[k]);
							}

							for(int k=0; k<V[j].points.size(); k++)
							{
								V[q].points.push_back(V[j].points[k]);
							}

							// now that a new cluster is created by merging V[i] and V[j]
							// delete V[i] and V[j]

							int last_pos = V.size() - 1;
							V[i] = V[last_pos];
							V.pop_back();

							int last_position = V.size() - 1;
							V[j] = V[last_position];
							V.pop_back();
							count = 1;
					}
				}
			}


		}

		return V;

}
void print1 (vector<cluster> P)
{
	cout<<"\n number of clusters are: "<<P.size()<<"\n";    //print clusters
	for(int i=0; i<P.size(); i++)
	{

		cout<<"Cluster["<<i<<"]\n";
		cout<<"data points of cluster: ";
		double s = P[i].points.size();
		for(int j=0; j<s; j++)
		{
		cout<<P[i].points[j]<<"\t";
		}

		cout<<"\n centroid of cluster is: "<<P[i].centroid<<"\n";
		cout<<"IDs of the nodes to which this cluster belongs: ";
		for(int j=0; j<P[i].nodeID.size(); j++)
		{
			cout<<P[i].nodeID[j]<<" ";
		}

	}
}


