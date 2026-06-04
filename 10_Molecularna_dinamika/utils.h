double gasdev() {//Generates numbers follo
	static bool available = false;
	static double gset;
	double fac, rsq, v1, v2;
	if (!available) {
		do {
			v1 = 2.0 * rand() / double(RAND_MAX) - 1.0;
			v2 = 2.0 * rand() / double(RAND_MAX) - 1.0;
			rsq = v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0 * log(rsq) / rsq);
		gset = v1 * fac;
		available = true;
		return v2 * fac;
	}
	else {
		available = false;
		return gset;
	}
}

double abs_velocity(int i) {//The squer of the velocity vector
	double vSqdSum = 0;
	for (int ij = 0; ij < 3; ij++) {
		vSqdSum += v[i][ij] * v[i][ij];
	}
	return vSqdSum;
}

void initialize() {
	//inicializacija leg
	int n = int(ceil(pow(N, 1.0 / 3.0)));//stevilo delcev v posameznih smereh (x,y, in z)
	double a = L / (n);                 //distance between 2 particles
	int p = 0;                         //counter
    FILE*data;
    data = fopen("init_pos.txt","w+");
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			for (int z = 0; z < n; z++) {
				if (p < N) {
					r[p][0] = (x + 0.5) * a;
					r[p][1] = (y + 0.5) * a;
					r[p][2] = (z + 0.5) * a;
                    fprintf(data,"%.4f\t%.4f\t%.4f\n",r[p][0],r[p][1],r[p][2]);
				}
				++p;
			}
		}
	}
    fclose(data);
	// initialize velocities
	for (int p = 0; p < N; p++) {
		for (int i = 0; i < 3; i++) {
			v[p][i] = gasdev();// vMax * (2 * rand() / double(RAND_MAX) - 1);
		}
	}
}

void final_position() {
	//inicializacija leg
	int n = int(ceil(pow(N, 1.0 / 3.0)));//stevilo delcev v posameznih smereh (x,y, in z)
    int p=0;
    FILE*data;
    data = fopen("final_pos.txt","w+");
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			for (int z = 0; z < n; z++) {
				if (p < N) {
                    fprintf(data,"%.4f\t%.4f\t%.4f\n",r[p][0],r[p][1],r[p][2]);
				}
				++p;
			}
		}
	}
    fclose(data);
}

void computeAccelerations() {
	//postavimo vrednosti pospeskov a 0
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 3; k++) {
			a[i][k] = 0;
		}
	}

	//zanka preko vseh parov
	for (int i = 0; i < N ; i++) {
		for (int j = i + 1; j < N; j++) {
			double rij[3];
			double rSqd = 0;
			for (int k = 0; k < 3; k++) {
				rij[k] = r[i][k] - r[j][k];
				if (abs(rij[k]) > 0.5*L) { //Periodični robni pogoji
					if (rij[k]>0) rij[k] -= L;
					else rij[k] += L;
				}
				rSqd += rij[k] * rij[k];
			}
			double f = 24 * (2 * pow(rSqd, -7) - pow(rSqd, -4));
			for (int k = 0; k < 3; k++) {
				a[i][k] += rij[k] * f;
				a[j][k] -= rij[k] * f;
			}
		}
		for (int k = 0; k < 3; k++) {
			if (r[i][k] > L) r[i][k] -= L;
			if (r[i][k] < 0) r[i][k] += L;
		}

	}
}

void velocityVerlet(double dt) {//Integration dynamics
	computeAccelerations();
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 3; k++) {
			r[i][k] += v[i][k] * dt + 0.5 * a[i][k] * dt * dt;
			v[i][k] += 0.5 * a[i][k] * dt;
		}
	}
	computeAccelerations();
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 3; k++) {
			v[i][k] += 0.5 * a[i][k] * dt;
		}
		abs_v[i] = sqrt(abs_velocity(i));
	}
}

void skaliranje_hitrosti(double T) {
    double vSqdSum = 0.0;
    double v_cm[3] = {0.0, 0.0, 0.0};
    
    // Določimo povprčeno itrost sistema oz. hitrost masnega središča
    for (int n = 0; n < N; n++) {
        for (int i = 0; i < 3; i++) {
            v_cm[i] += v[n][i];
        }
    }
    
    for (int i = 0; i < 3; i++) {
        v_cm[i] /= N;
    }
    
    /*
	Ker želimo da velja zakon o ohranitvi gibalne količine napravimo korekcijo
	vrednosti hitrosti centriramo okoli hitrosti masnega središča
	*/
    for (int n = 0; n < N; n++) {
        for (int i = 0; i < 3; i++) {
            v[n][i] -= v_cm[i]; // Remove CM motion
            vSqdSum += v[n][i] * v[n][i];
        }
    }
    
    // SEdaj pa skaliramo hitrosti glede na temperaturo
    double lambda = sqrt(3 * (N - 1) * T / vSqdSum);
    for (int n = 0; n < N; n++) {
        for (int i = 0; i < 3; i++) {
            v[n][i] *= lambda;
        }
    }
}

double instantaneousTemperature() {//Compute the instant temperature
	double sum = 0;
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < 3; k++) {
			sum += v[i][k] * v[i][k];
		}
	}
	return sum / (3 * (N - 1));
}

string ustvari_filename(int argc, char* argv[], double T, string base_name){
	// Pretvorim T v string
	ostringstream t_ss;
	t_ss << T;
	string t_str = t_ss.str();

	size_t pos = t_str.find('.');
	//string::npos -> če ni ničle v stringu t_str (torej ni decimalne vejice)
	while (pos != string::npos) {
		t_str.replace(pos, 1, "dp"); // zamenjaj decimalno vejico z "dp", 1 znak
		pos = t_str.find('.', pos + 2); // premakni se čez novo vstavljen "dp"
	}

	string filename_str = "./RESULTS/" + t_str + "_T_" + base_name + ".txt";
	char filename[50];
	strcpy(filename, filename_str.c_str());
	FILE* pisi = fopen(filename, "w+");
	fclose(pisi);
	return filename;
}

double* sorted_dist(const double r[N][3], int particle) {
    // Allocate and initialize distances array
    double* distances = new double[N];
    for (int i = 0; i < N; i++) {
        distances[i] = 0.0;
        for (int j = 0; j < 3; j++) {
            double diff = r[particle][j] - r[i][j];
            distances[i] += diff * diff;
        }
    }
    
    // Copy distances to sorted_array
    double* sorted_array = new double[N];
    for (int i = 0; i < N; i++) {
        sorted_array[i] = distances[i];
    }
    
    // Manual selection sort
    for (int i = 0; i < N - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < N; j++) {
            if (sorted_array[j] < sorted_array[min_index]) {
                min_index = j;
            }
        }
        std::swap(sorted_array[i], sorted_array[min_index]);
    }
    
    delete[] distances;
    return sorted_array; //Pazi. Prva vrednost je 0, ke rot je razdalja do samega sebe.
}

double avrage_min_nn_distance(double r[N][3], int nn){
	/*
	int nn - parameter, ki pove. koliko najbljizjih sosedov bomo upostevali
	pri določevanju povprečne najkrajše razdalje.
	*/
	double r_min_sum = 0;
	for (int i = 0; i < N; i++) {
		double* sorted_array = sorted_dist(r, i);
		for (int j = 1; j <= nn; j++) {
			r_min_sum += sqrt(sorted_array[j]);
		}
		delete[] sorted_array;
	}
	return r_min_sum / (N * nn);
}

int read_positions_from_file(const string& filename, vector<vector<vector<double>>>& positions, const int N) {
    ifstream podatki(filename);
    if (!podatki) {
        printf("Napaka pri odpiranju datoteke: %s\n", filename.c_str());
        return 0;
    }

    string vrstica;
    double time_ref = 0, time_step, x, y, z, velocity;
    vector<vector<double>> current_positions;
    
    while (getline(podatki, vrstica)) {
        stringstream ss(vrstica);
        ss >> time_step >> x >> y >> z >> velocity;

        if (time_step != time_ref) {
            if (!current_positions.empty()) {
                positions.push_back(current_positions);
            }
            current_positions.clear();
            time_ref = time_step;
        }

        current_positions.push_back({x, y, z});
    }

    if (!current_positions.empty()) {
        positions.push_back(current_positions);
    }

	podatki.close();
    return positions.size();
}

void compute_average_rdf(int N, double L, double rho, int int_steps, const string& pisi_gr, const vector<vector<vector<double>>>& positions) {
    double dr = 0.01;  // Bin width
    double r_max = L / 2;  // Maximum distance to consider (half of the box size)
    if (r_max > 10.0) r_max = 10.0;  // Limit the maximum distance for RDF
    int n_bins = int(r_max / dr);  // Number of bins

    // Array to store the RDF values
    vector<double> g(n_bins, 0.0);

    // Loop over all time steps and compute RDF for each time step
    for (int time = 0; time < int_steps; time++) {
        const auto& current_positions = positions[time];

        // Compute pairwise distances for the current time step
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                double rij[3];
                double rSqd = 0;
                // Compute pairwise distance considering periodic boundary conditions
                for (int k = 0; k < 3; k++) {
                    rij[k] = current_positions[i][k] - current_positions[j][k];

                    if (abs(rij[k]) > 0.5 * L) {  // Apply periodic boundary conditions
                        if (rij[k] > 0){
							rij[k] -= L;
						}else{
							rij[k] += L;
						}
                    }

                    rSqd += rij[k] * rij[k];
                }

                double distance = sqrt(rSqd);
                if (distance < r_max) {
                    int bin = int(distance / dr);  
                    g[bin] += 2;  // Simetrija i - j = i - j
                }
            }
        }
    }

    // Normalizacija
    double volume_shell = 4.0 / 3.0 * PI * pow(r_max, 3);
    double normalization_factor = int_steps * volume_shell;

    for (int i = 0; i < n_bins; i++) {
        double r = i * dr;
        double r1 = r + dr;
        double vshell = 4.0 / 3.0 * PI * (pow(r1, 3) - pow(r, 3));
        g[i] /= (normalization_factor * vshell * rho);
    }

    ofstream rdf_file(pisi_gr);
    if (!rdf_file) {
        cerr << "Error opening file: " << pisi_gr << endl;
        return;
    }

    for (int i = 0; i < n_bins; i++) {
        double r = i * dr;
        rdf_file << r << "\t" << g[i] << endl;
    }

    rdf_file.close();
}

void compute_rdf(const string& filename, const string& pisi_gr) {
    vector<vector<vector<double>>> positions;  // 3D vektor [time][particle][coordinate]

    int int_steps = read_positions_from_file(filename, positions, N);
    compute_average_rdf(N, L, rho, int_steps, pisi_gr, positions);
}