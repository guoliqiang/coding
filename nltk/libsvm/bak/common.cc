// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-05 11:16:27
// File  : common.cc
// Brief :

#include "../public/common.h"
#include "../public/svn_problem.h"
#include "../public/svn_parameter.h"
#include "../public/svn_node.h"
#include "../public/svn_model.h"


namespace nltk {

// decision_function
struct DecisionFunction {
	double *alpha_;
	double rho_;	
};


// Platt's binary SVM Probablistic Output: an improvement from Lin et al.
static void SigmoidTrain(int l, const double *dec_values,
                         const double *labels, 
	                       double& A, double& B) {
	double prior1 = 0;
  double prior0 = 0;
	int i;

	for (i = 0; i < l; i++) {
		if (labels[i] > 0) {
      prior1+=1;
		} else {
      prior0+=1;
    }
  }
	
	int max_iter = 100;	// Maximal number of iterations
	double min_step = 1e-10;	// Minimal step taken in line search
	double sigma = 1e-12;	// For numerically strict PD of Hessian
	double eps = 1e-5;
	double hiTarget = (prior1 + 1.0) / (prior1 + 2.0);
	double loTarget = 1 / (prior0 + 2.0);
	double *t = Malloc(double, l);
	double fApB, p, q, h11, h22, h21, g1, g2, det, dA, dB, gd, stepsize;
	double newA, newB, newf, d1, d2;
	int iter; 
	
	// Initial Point and Initial Fun Value
	A=0.0;
  B=log((prior0 + 1.0)/(prior1 + 1.0));
	double fval = 0.0;

	for (i = 0; i < l; i++) {
		if (labels[i]>0) {
      t[i]=hiTarget;
    } else {
      t[i]=loTarget;
    }
		fApB = dec_values[i] * A + B;
		if (fApB>=0) {
			fval += t[i] * fApB + log(1 + exp(-fApB));
		} else {
			fval += (t[i] - 1) * fApB + log(1 + exp(fApB));
    }
	}
	for (iter = 0; iter < max_iter; iter++) {
		// Update Gradient and Hessian (use H' = H + sigma I)
		h11 = sigma; // numerically ensures strict PD
		h22 = sigma;
		h21 = 0.0;
    g1 = 0.0;
    g2 = 0.0;
		for (i = 0; i < l; i++) {
			fApB = dec_values[i]*A + B;
			if (fApB >= 0) {
				p=exp(-fApB) / (1.0 + exp(-fApB));
				q=1.0 / (1.0 + exp(-fApB));
			} else {
				p=1.0 / (1.0 + exp(fApB));
				q=exp(fApB) / (1.0 + exp(fApB));
			}
			d2 = p * q;
			h11 += dec_values[i] * dec_values[i] * d2;
			h22 += d2;
			h21 += dec_values[i] * d2;
			d1 = t[i] - p;
			g1 += dec_values[i] * d1;
			g2 += d1;
		}

		// Stopping Criteria
		if (fabs(g1) < eps && fabs(g2) < eps) break;

		// Finding Newton direction: -inv(H') * g
		det = h11 * h22 - h21 * h21;
		dA = -(h22 * g1 - h21 * g2) / det;
		dB = -(-h21 * g1 + h11 * g2) / det;
		gd = g1 * dA + g2 * dB;


		stepsize = 1;		// Line Search
		while (stepsize >= min_step) {
			newA = A + stepsize * dA;
			newB = B + stepsize * dB;

			// New function value
			newf = 0.0;
			for (i = 0; i < l; i++) {
				fApB = dec_values[i] * newA + newB;
				if (fApB >= 0) {
					newf += t[i] * fApB + log(1 + exp(-fApB));
				} else {
					newf += (t[i] - 1) * fApB + log(1 + exp(fApB));
        }
			}
			// Check sufficient decrease
			if (newf < fval + 0.0001 * stepsize * gd) {
				A = newA;
        B = newB;
        fval = newf;
				break;
			} else {
				stepsize = stepsize / 2.0;
      }
		}  // while

		if (stepsize < min_step) {
			info("Line search fails in two-class probability estimates\n");
			break;
		}
	}

	if (iter >= max_iter) {
		info("Reaching maximal iterations in two-class probability estimates\n");
  }
	free(t);
}

double SigmoidPredict(double decision_value,
                      double A, double B) {
	double fApB = decision_value * A + B;
	// 1-p used later; avoid catastrophic cancellation
	if (fApB >= 0) {
		return exp(-fApB) / (1.0 + exp(-fApB));
	} else {
		return 1.0 / (1 + exp(fApB));
  }
}

// Method 2 from the multiclass_prob paper by Wu, Lin, and Weng
void MulticlassProbability(int k, double **r, double *p) {
	int t,j;
	int iter = 0;
  int max_iter = max(100, k);
	double ** Q = Malloc(double *, k);
	double * Qp = Malloc(double, k);
	double pQp;
  double eps = 0.005 / k;
	
	for (t = 0; t < k; t++) {
		p[t] = 1.0/k;  // Valid if k = 1
		Q[t] = Malloc(double,k);
		Q[t][t] = 0;
		for (j = 0; j < t; j++) {
			Q[t][t] += r[j][t] * r[j][t];
			Q[t][j] = Q[j][t];
		}
		for (j = t+1; j < k; j++) {
			Q[t][t] += r[j][t] * r[j][t];
			Q[t][j] =- r[j][t] * r[t][j];
		}
	}
	for (iter = 0; iter < max_iter; iter++) {
		// stopping condition, recalculate QP,pQP for numerical accuracy
		pQp = 0;
		for (t = 0; t < k; t++) {
			Qp[t] = 0;
			for (j = 0; j < k; j++) {
        Qp[t] += Q[t][j] * p[j];
      }
			pQp += p[t]*Qp[t];
		}
		double max_error = 0;
		for (t = 0; t < k; t++) {
			double error = fabs(Qp[t] - pQp);
			if (error > max_error) max_error = error;
		}
		if (max_error<eps) break;
		
		for (t = 0; t < k; t++) {
			double diff = (-Qp[t] + pQp) / Q[t][t];
			p[t] += diff;
			pQp = (pQp + diff * (diff * Q[t][t] + 2 * Qp[t])) / (1 + diff) / ( 1 + diff);
			for (j = 0; j < k; j++) {
				Qp[j] = (Qp[j] + diff * Q[t][j]) / (1 + diff);
				p[j] /= (1 + diff);
			}
		}
	}
	if (iter >= max_iter) {
    info("Exceeds max_iter in multiclass_prob\n");
  }
	for(t = 0; t < k; t++) {
    free(Q[t]);
  }
	free(Q);
	free(Qp);
}

DecisionFunction TrainOne(const SvmProblem *prob,
                          const SvmParameter *param,
                          double Cp, double Cn) {
	double *alpha = Malloc(double, prob->l_);
	Solver::SolutionInfo si;
	switch(param->svm_type) {
		case C_SVC:
			SolveCSvc(prob, param, alpha, &si, Cp, Cn);
	}

	LOG(INFO) << "obj =" << si.obj_ << "rho =" << si.rho_;
	// output SVs
	int nSV = 0;
	int nBSV = 0;
	for(int i = 0; i< prob->l_; i++) {
		if(fabs(alpha[i]) > 0) {
			++nSV;
			if(prob->y[i] > 0) {
				if(fabs(alpha[i]) >= si.upper_bound_p_) ++nBSV;
			} else {
				if(fabs(alpha[i]) >= si.upper_bound_n_) ++nBSV;
			}
		}
	}  // for

	LOG(INFO) << "nSV =" << nSV << "nBSV =" << nBSV;

	DecisionFunction f;
	f.alpha_ = alpha;
	f.rho_ = si.rho_;
	return f;
}

// construct and solve various formulations
void SolveCSvc(const SvmProblem *prob,
               const SvmParameter* param,
               double *alpha,
               Solver::SolutionInfo* si,
               double Cp, double Cn) {
	int l = prob->l_;
	double * minus_ones = new double[l];
	schar * y = new schar[l];

	int i;
	for(i = 0; i < l; i++) {
		alpha[i] = 0;
		minus_ones[i] = -1;
		if(prob->y[i] > 0) {
      y[i] = +1;
    } else {
      y[i] = -1;
    }
	}

	Solver s;
	s.Solve(l, SVC_Q(*prob, *param, y),
          minus_ones, y,
		      alpha,
          Cp, Cn,
          param->eps, si, param->shrinking);

	double sum_alpha=0;
	for(i = 0; i < l; i++) {
    sum_alpha += alpha[i];
  }

	if (Cp == Cn) {
    info("nu = %f\n", sum_alpha / (Cp * prob->l_));
  }

	for(i = 0; i < l; i++) {
    alpha[i] *= y[i];
  }

	delete[] minus_ones;
	delete[] y;
}

// Cross-validation decision values for probability estimates
void BinarySvcProbability(const SvmProblem *prob,
                          const SvmParameter *param,
                          double Cp, double Cn,
                          double& probA, double& probB) {
	int i;
	int nr_fold = 5;
	int *perm = Malloc(int,prob->l);
	double *dec_values = Malloc(double,prob->l);

	// random shuffle
	for(i = 0; i < prob->l_; i++) {
    perm[i] = i;
  }
	for(i = 0; i < prob->l_; i++) {
		int j = i + rand() % (prob->l_ - i);
		swap(perm[i], perm[j]);
	}
	for(i = 0; i < nr_fold; i++) {
		int begin = i * prob->l_ / nr_fold;
		int end = (i + 1) * prob->l_ / nr_fold;
		int j,k;
		struct SvmProblem subprob;

		subprob.l_ = prob->l_ - (end - begin);
		subprob.x_ = Malloc(SvmNode*, subprob.l_);
		subprob.y_ = Malloc(double, subprob.l_);
			
		k = 0;
		for(j = 0; j < begin; j++) {
			subprob.x_[k] = prob->x_[perm[j]];
			subprob.y_[k] = prob->y_[perm[j]];
			++k;
		}
		for(j = end; j < prob->l_; j++) {
			subprob.x_[k] = prob->x_[perm[j]];
			subprob.y_[k] = prob->y_[perm[j]];
			++k;
		}
		int p_count=0;
    int n_count=0;
		for(j = 0; j < k; j++) {
			if(subprob.y_[j]>0) {
				p_count++;
			} else {
				n_count++;
      }
    }

		if(p_count==0 && n_count==0) {
			for(j = begin; j < end; j++) {
        dec_values[perm[j]] = 0;
      }
    } else if(p_count > 0 && n_count == 0) {
			for(j = begin; j < end; j++) {
        dec_values[perm[j]] = 1;
      }
		} else if(p_count == 0 && n_count > 0) {
			for( j = begin; j < end; j++) {
        dec_values[perm[j]] = -1;
      }
		} else {
			SvmParameter subparam = *param;
			subparam.probability_ = 0;
			subparam.C_ = 1.0;
			subparam.nr_weight_ = 2;
			subparam.weight_label_ = Malloc(int, 2);
			subparam.weight_ = Malloc(double, 2);
			subparam.weight_label_[0] = +1;
			subparam.weight_label_[1] = -1;
			subparam.weight_[0] = Cp;
			subparam.weight_[1] = Cn;
			struct SvmModel *submodel = Train(&subprob, &subparam);
			for(j = begin; j < end; j++) {
				PredictValues(submodel, prob->x_[perm[j]], &(dec_values[perm[j]])); 
				// ensure +1 -1 order; reason not using CV subroutine
				dec_values[perm[j]] *= submodel->label_[0];
			}		
			FreeAndDestroy_model(&submodel);
			DestroyParam(&subparam);
		}
		free(subprob.x_);
		free(subprob.y_);
	}		
	SigmoidTrain(prob->l_, dec_values, prob->y_, probA, probB);
	free(dec_values);
	free(perm);
}

// label: label name, start: begin of each class,
// count: #data of classes,
// perm: indices to the original data
// perm, length l, must be allocated before calling this subroutine
static void GroupC`lasses(const SvmProblem *prob,
                          int *nr_class_ret,
                          int **label_ret,
                          int **start_ret,
                          int **count_ret, int *perm) {
	int l = prob->l_;
	int max_nr_class = 16;
	int nr_class = 0;
	int *label = Malloc(int, max_nr_class);
	int *count = Malloc(int, max_nr_class);
	int *data_label = Malloc(int, l);	
	int i;

	for(i = 0; i < l; i++) {
		int this_label = (int)prob->y_[i];
		int j;
		for(j = 0;j < nr_class; j++) {
			if(this_label == label[j]) {
				++count[j];
				break;
			}
		}
		data_label[i] = j;
		if(j == nr_class) {
			if(nr_class == max_nr_class) {
				max_nr_class *= 2;
				label = (int *)realloc(label, max_nr_class*sizeof(int));
				count = (int *)realloc(count, max_nr_class*sizeof(int));
			}
			label[nr_class] = this_label;
			count[nr_class] = 1;
			++nr_class;
		}
	}

	//
	// Labels are ordered by their first occurrence in the training set. 
	// However, for two-class sets with -1/+1 labels and -1 appears first, 
	// we swap labels to ensure that internally the binary SVM has positive data corresponding to the +1 instances.
	//
	if (nr_class == 2 && label[0] == -1 && label[1] == 1) {
		swap(label[0], label[1]);
		swap(count[0], count[1]);
		for(i = 0; i < l; i++) {
			if(data_label[i] == 0) {
				data_label[i] = 1;
			} else {
				data_label[i] = 0;
      }
		}
	}

	int *start = Malloc(int, nr_class);
	start[0] = 0;
	for(i = 1; i < nr_class; i++) {
    start[i] = start[i - 1] + count[i - 1];
  }
	for(i = 0; i < l; i++) {
		perm[start[data_label[i]]] = i;
		++start[data_label[i]];
	}
	start[0] = 0;
	for(i=1;i<nr_class;i++) {
    start[i] = start[i - 1] + count[i - 1];
  }

	*nr_class_ret = nr_class;
	*label_ret = label;
	*start_ret = start;
	*count_ret = count;
  LOG(INFO) << *nr_class_ret;
  for (i = 0; i < nr_class; i++)
    LOG(INFO) << label[i];
  for (i = 0; i < nr_class; i++)
    LOG(INFO) << count[i];
  for (i = 0; i < nr_class; i++)
    LOG(INFO) << start[i];
  for (i = 0; i < l; i++)
    LOG(INFO) << perm[i];
	free(data_label);
}



SvmModel * Train(const SvmProblem * prob,
                 const SvmParameter * param) {
  SvmModel * model = Malloc(SvmModel, 1);
  model->param_ = *param;
	model->free_sv_ = 0;	// XXX

  // classification
  int l = prob->l_;
	int nr_class;
	int * label = NULL;
	int * start = NULL;
	int * count = NULL;
	int * perm = Malloc(int, l);

	// group training data of the same class
	GroupClasses(prob, &nr_class, &label,
               &start, &count, perm);
	// [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1968)] 2  == nr_class
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1970)] 1  == lable[0]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1970)] -1 == lable[1]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1972)] 3  == count[0]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1972)] 7  == count[1]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1974)] 0  == start[0]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1974)] 3  == start[1]
  //
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 4  == per[0]
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 8
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 9
  
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 0
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 1
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 2
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 3
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 5
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 6
  // [17119:0905/152617:INFO:nltk/libsvm/internal/svm.cc(1976)] 7  == pre[10]
  if(nr_class == 1) {
    LOG(WARN) << "WARNING: training data in only"
              << " one class. See README for details.\n";
  }
		
	SvmNode ** x = Malloc(SvmNode *, l);
	for(int i = 0; i < l; i++) {
     x[i] = prob->x_[perm[i]];
  }

	// calculate weighted C
	double *weighted_C = Malloc(double, nr_class);
	for(int i = 0; i < nr_class; i++) {
    weighted_C[i] = param->C_;
  }
	for(int i = 0; i < param->nr_weight_; i++) {	
		int j = 0;
    for(j = 0; j < nr_class; j++){
			if(param->weight_label_[i] == label[j]) break;
    }
		if(j == nr_class) {
			LOG(WARN) << "WARNING: class label" << param->weight_label_[i]
                << " specified in weight is not found";
		} else {
			weighted_C[j] * = param->weight_[i];
    }
	}

  // train k*(k-1)/2 models
		
  bool *nonzero = Malloc(bool, l);
  for(i = 0; i < l; i++) {
    nonzero[i] = false;
  }
  DecisionFunction *f = Malloc(DecisionFunction,
                               nr_class * (nr_class-1) / 2);

  double *probA = NULL;
  double *probB = NULL;
  if (param->probability_) {
			probA = Malloc(double, nr_class * (nr_class-1) / 2);
			probB = Malloc(double, nr_class * (nr_class-1) / 2);
  }

	int p = 0;
	for(i = 0; i < nr_class; i++) {
    for(int j = i + 1; j < nr_class; j++) {
			SvmProblem sub_prob;
			int si = start[i];
      int sj = start[j];
  		int ci = count[i];
      int cj = count[j];
			sub_prob.l_ = ci + cj;
			sub_prob.x_ = Malloc(SvmNode *, sub_prob.l_);
			sub_prob.y_ = Malloc(double, sub_prob.l_);
			int k;
			for(k = 0; k < ci; k++) {
				sub_prob.x_[k] = x[si + k];
				sub_prob.y_[k] = +1;
			}
			for(k = 0; k < cj; k++) {
				sub_prob.x_[ci + k] = x[sj + k];
				sub_prob.y_[ci + k] = -1;
			}

			if(param->probability_) {
				BinarySvcProbability(&sub_prob, param,
                             weighted_C[i], weighted_C[j],
                             probA[p], probB[p]);
      }

			f[p] = TrainOne(&sub_prob, param,
                      weighted_C[i], weighted_C[j]);
			for(k = 0; k < ci; k++) {
				if(!nonzero[si + k] && fabs(f[p].alpha_[k]) > 0) {
           nonzero[si + k] = true;
         }
      }
			for(k = 0; k < cj; k++) {
				if(!nonzero[sj + k] && fabs(f[p].alpha_[ci + k]) > 0) {
           nonzero[sj + k] = true;
        }
      }
			free(sub_prob.x_);
			free(sub_prob.y_);
			++p;
		}
  }  // for

	// build output
	model->nr_class_ = nr_class;
	model->label_ = Malloc(int, nr_class);
	for(i = 0; i < nr_class; i++) {
    model->label_[i] = label[i];
  }
		
	model->rho_ = Malloc(double, nr_class * (nr_class - 1) / 2);
	for(i = 0; i <nr_class * (nr_class -1 ) / 2; i++) {
    model->rho_[i] = f[i].rho_;
  }

	if(param->probability_) {
		model->probA = Malloc(double, nr_class * (nr_class - 1) / 2);
		model->probB = Malloc(double, nr_class * (nr_class - 1) / 2);
		for(i = 0; i < nr_class * (nr_class - 1) / 2; i++) {
			model->probA[i] = probA[i];
			model->probB[i] = probB[i];
		}
	} else{
		model->probA=NULL;
		model->probB=NULL;
	}

	int total_sv = 0;
	int * nz_count = Malloc(int, nr_class);
	model->nSV_ = Malloc(int, nr_class);
	for(i = 0; i < nr_class; i++) {
		int nSV = 0;
		for(int j = 0; j < count[i]; j++) {
			if(nonzero[start[i] + j]) {	
				++nSV;
				++total_sv;
			}
    }
		model->nSV_[i] = nSV;
		nz_count[i] = nSV;
	}
		
	info("Total nSV = %d\n", total_sv);

	model->l_ = total_sv;
	model->SV_ = Malloc(SvmNode *, total_sv);
	model->sv_indices_ = Malloc(int, total_sv);
	p = 0;
	for(i = 0; i < l; i++) {
		if(nonzero[i]) {
			model->SV_[p] = x[i];
			model->sv_indices_[p++] = perm[i] + 1;
		}
  }

	int *nz_start = Malloc(int, nr_class);
	nz_start[0] = 0;
	for( i = 1; i < nr_class; i++) {
    nz_start[i] = nz_start[i - 1] + nz_count[i - 1];
  }

	model->sv_coef_ = Malloc(double *, nr_class-1);
	for(i = 0; i < nr_class - 1; i++) {
    model->sv_coef_[i] = Malloc(double, total_sv);
  }

	p = 0;
	for( i = 0; i < nr_class; i++) {
		for(int j = i + 1; j < nr_class; j++) {
			// classifier (i,j): coefficients with
			// i are in sv_coef[j-1][nz_start[i]...],
			// j are in sv_coef[i][nz_start[j]...]
			int si = start[i];
			int sj = start[j];
			int ci = count[i];
			int cj = count[j];
				
			int q = nz_start[i];
			int k;
			for(k = 0; k < ci; k++) {
				if(nonzero[si+k]) model->sv_coef_[j - 1][q++] = f[p].alpha_[k];
      }
			q = nz_start[j];
			for( k = 0; k < cj; k++) {
				if(nonzero[sj+k]) model->sv_coef_[i][q++] = f[p].alpha_[ci + k];
      }
	  	++p;
		}
  }
		
	free(label);
	free(probA);
	free(probB);
	free(count);
	free(perm);
	free(start);
	free(x);
	free(weighted_C);
	free(nonzero);
	for(i = 0; i < nr_class * (nr_class - 1) / 2; i++) {
    free(f[i].alpha_);
  }
	free(f);
	free(nz_count);
	free(nz_start);
  return model;
}

double PredictValues(const SvmModel *model,
                     const SvmNode *x,
                     double* dec_values) {
	int i;
  int nr_class = model->nr_class_;
  int l = model->l_;
		
  double *kvalue = Malloc(double, l);
	for(i = 0; i < l; i++) {
		kvalue[i] = Kernel::Kfunction(x, model->SV_[i], model->param);
  }

	int *start = Malloc(int, nr_class);
	start[0] = 0;
	for(i = 1; i< nr_class; i++) {
		start[i] = start[i - 1] + model->nSV[i - 1];
  }

	int *vote = Malloc(int, nr_class);
	for(i = 0; i < nr_class; i++) {
    vote[i] = 0;
  }

	int p = 0;
	for(i = 0; i < nr_class; i++) {
		for(int j = i + 1; j < nr_class; j++) {
			double sum = 0;
			int si = start[i];
			int sj = start[j];
			int ci = model->nSV_[i];
			int cj = model->nSV_[j];
				
			int k;
			double *coef1 = model->sv_coef_[j - 1];
			double *coef2 = model->sv_coef_[i];
			for(k = 0; k < ci; k++) {
        sum += coef1[si + k] * kvalue[si + k];
      }
			for(k = 0;k < cj; k++) {
        sum += coef2[sj+k] * kvalue[sj+k];
      }
			sum -= model->rho[p];
			dec_values[p] = sum;

			if(dec_values[p] > 0) {
				++vote[i];
			} else {
				++vote[j];
      }
			p++;
		}
  }

	int vote_max_idx = 0;
	for(i = 1; i < nr_class; i++) {
		if(vote[i] > vote[vote_max_idx]) {
			vote_max_idx = i;
    }
  }

  free(kvalue);
	free(start);
	free(vote);
	return model->label_[vote_max_idx];
}

double Predict(const SvmModel *model, const SvmNode *x) {
	int nr_class = model->nr_class_;
	double *dec_values;
  dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
	double pred_result = PredictValues(model, x, dec_values);
	free(dec_values);
	return pred_result;
}

double PredictProbability(const SvmModel *model,
                          const SvmNode *x,
                          double *prob_estimates) {
	if (model->param.svm_type == C_SVC &&
	    model->probA != NULL && model->probB != NULL) {
		int i;
		int nr_class = model->nr_class_;
		double *dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
		PredictValues(model, x, dec_values);

		double min_prob = 1e-7;
		double ** pairwise_prob = Malloc(double *, nr_class);
		for(i = 0; i < nr_class; i++) {
			pairwise_prob[i] = Malloc(double, nr_class);
    }
		int k = 0;
		for(i = 0; i < nr_class; i++) {
			for(int j = i + 1; j < nr_class; j++) {
				pairwise_prob[i][j] = min(max(SigmoidPredict(dec_values[k],
                                                     model->probA_[k],
                                                     model->probB_[k]),
                                      min_prob),
                                1 - min_prob);
				pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
				k++;
			}
    }
		MulticlassProbability(nr_class, pairwise_prob, prob_estimates);

		int prob_max_idx = 0;
		for(i = 1;i < nr_class; i++) {
			if(prob_estimates[i] > prob_estimates[prob_max_idx]) {
        prob_max_idx = i;
      }
    }
		for( i = 0; i < nr_class; i++) {
      free(pairwise_prob[i]);
    }
		free(dec_values);
		free(pairwise_prob);	     
		return model->label_[prob_max_idx];
	} else {
		return Predict(model, x);
  }
}

static char *line = NULL;
static int max_line_len;

char* readline(FILE *input) {
	int len;
	if(fgets(line,max_line_len,input) == NULL) return NULL;
	while(strrchr(line,'\n') == NULL) {
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL) break;
	}
	return line;
}

svm_model * svm_load_model(const char *model_file_name) {
	FILE *fp = fopen(model_file_name,"rb");
	if(fp==NULL) return NULL;

	char *old_locale = strdup(setlocale(LC_ALL, NULL));
	setlocale(LC_ALL, "C");

	// read parameters
	svm_model *model = Malloc(svm_model,1);
	svm_parameter& param = model->param;
	model->rho = NULL;
	model->probA = NULL;
	model->probB = NULL;
	model->sv_indices = NULL;
	model->label = NULL;
	model->nSV = NULL;

	char cmd[81];
	while(1) {
		fscanf(fp,"%80s",cmd);
		if(strcmp(cmd,"svm_type")==0) {
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;svm_type_table[i];i++) {
				if(strcmp(svm_type_table[i],cmd)==0) {
					param.svm_type=i;
					break;
				}
			}
			if(svm_type_table[i] == NULL) {
				fprintf(stderr,"unknown svm type.\n");
				setlocale(LC_ALL, old_locale);
				free(old_locale);
				free(model->rho);
				free(model->label);
				free(model->nSV);
				free(model);
				return NULL;
			}
		} else if(strcmp(cmd,"kernel_type")==0) {		
			fscanf(fp,"%80s",cmd);
			int i;
			for(i=0;kernel_type_table[i];i++) {
				if(strcmp(kernel_type_table[i],cmd)==0) {
					param.kernel_type=i;
					break;
				}
			}
			if(kernel_type_table[i] == NULL) {
				fprintf(stderr,"unknown kernel function.\n");
				setlocale(LC_ALL, old_locale);
				free(old_locale);
				free(model->rho);
				free(model->label);
				free(model->nSV);
				free(model);
				return NULL;
			}
		} else if(strcmp(cmd,"degree")==0) {
			fscanf(fp,"%d",&param.degree);
		} else if(strcmp(cmd,"gamma")==0) {
			fscanf(fp,"%lf",&param.gamma);
		} else if(strcmp(cmd,"coef0")==0) {
			fscanf(fp,"%lf",&param.coef0);
		} else if(strcmp(cmd,"nr_class")==0) {
			fscanf(fp,"%d",&model->nr_class);
		} else if(strcmp(cmd,"total_sv")==0) {
			fscanf(fp,"%d",&model->l);
		} else if(strcmp(cmd,"rho")==0) {
			int n = model->nr_class * (model->nr_class-1)/2;
			model->rho = Malloc(double,n);
			for(int i=0;i<n;i++) fscanf(fp,"%lf",&model->rho[i]);
		} else if(strcmp(cmd,"label")==0) {
			int n = model->nr_class;
			model->label = Malloc(int,n);
			for(int i=0;i<n;i++) fscanf(fp,"%d",&model->label[i]);
		} else if(strcmp(cmd,"probA")==0) {
			int n = model->nr_class * (model->nr_class-1)/2;
			model->probA = Malloc(double,n);
			for(int i=0;i<n;i++) fscanf(fp,"%lf",&model->probA[i]);
		} else if(strcmp(cmd,"probB")==0) {
			int n = model->nr_class * (model->nr_class-1)/2;
			model->probB = Malloc(double,n);
			for(int i=0;i<n;i++) fscanf(fp,"%lf",&model->probB[i]);
		} else if(strcmp(cmd,"nr_sv")==0) {
			int n = model->nr_class;
			model->nSV = Malloc(int,n);
			for(int i=0;i<n;i++) fscanf(fp,"%d",&model->nSV[i]);
		} else if(strcmp(cmd,"SV")==0) {
			while(1) {
				int c = getc(fp);
				if(c==EOF || c=='\n') break;	
			}
			break;
		} else {
			fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
			setlocale(LC_ALL, old_locale);
			free(old_locale);
			free(model->rho);
			free(model->label);
			free(model->nSV);
			free(model);
			return NULL;
		}
	}

	// read sv_coef and SV
	int elements = 0;
	long pos = ftell(fp);

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	char *p,*endptr,*idx,*val;

	while(readline(fp)!=NULL) {
		p = strtok(line,":");
		while(1) {
			p = strtok(NULL,":");
			if(p == NULL) break;
			++elements;
		}
	}
	elements += model->l;

	fseek(fp,pos,SEEK_SET);

	int m = model->nr_class - 1;
	int l = model->l;
	model->sv_coef = Malloc(double *,m);
	int i;
	for(i=0;i<m;i++) model->sv_coef[i] = Malloc(double,l);
	model->SV = Malloc(svm_node*,l);
	svm_node *x_space = NULL;
	if(l>0) x_space = Malloc(svm_node,elements);

	int j=0;
	for(i=0;i<l;i++) {
		readline(fp);
		model->SV[i] = &x_space[j];

		p = strtok(line, " \t");
		model->sv_coef[0][i] = strtod(p,&endptr);
		for(int k=1;k<m;k++) {
			p = strtok(NULL, " \t");
			model->sv_coef[k][i] = strtod(p,&endptr);
		}

		while(1) {
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if(val == NULL)
				break;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			x_space[j].value = strtod(val,&endptr);

			++j;
		}
		x_space[j++].index = -1;
	}
	free(line);

	setlocale(LC_ALL, old_locale);
	free(old_locale);

	if (ferror(fp) != 0 || fclose(fp) != 0) return NULL;

	model->free_sv = 1;	// XXX
	return model;
}

void svm_free_model_content(svm_model* model_ptr) {
	if(model_ptr->free_sv && model_ptr->l > 0 &&
    model_ptr->SV != NULL) {
    free((void *)(model_ptr->SV[0]));
  }
	
  if(model_ptr->sv_coef) {
		for(int i=0;i<model_ptr->nr_class-1;i++) {
      free(model_ptr->sv_coef[i]);
    }
	}

	free(model_ptr->SV);
	model_ptr->SV = NULL;

	free(model_ptr->sv_coef);
	model_ptr->sv_coef = NULL;

	free(model_ptr->rho);
	model_ptr->rho = NULL;

	free(model_ptr->label);
	model_ptr->label= NULL;

	free(model_ptr->probA);
	model_ptr->probA = NULL;

	free(model_ptr->probB);
	model_ptr->probB= NULL;

	free(model_ptr->sv_indices);
	model_ptr->sv_indices = NULL;

	free(model_ptr->nSV);
	model_ptr->nSV = NULL;
}

void svm_free_and_destroy_model(svm_model** model_ptr_ptr) {
	if(model_ptr_ptr != NULL && *model_ptr_ptr != NULL) {
		svm_free_model_content(*model_ptr_ptr);
		free(*model_ptr_ptr);
		*model_ptr_ptr = NULL;
	}
}

void svm_destroy_param(svm_parameter* param) {
	free(param->weight_label);
	free(param->weight);
}

}  // namespace nltk

