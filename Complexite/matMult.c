#define n 50

int d[n];

int Mult(int i, int j) {
	int min, tmp;
	if (i==j) return 0;
	else {
		min = Mult(i,i) + Mult(i+1,j) + d[i]*d[i+1]*d[j+1];
		for (int k = i+1; k<j; k++) {
			tmp = Mult(i,k) + Mult(k+1,j) + d[i]*d[k+1]*d[j+1];
			if (tmp < min) min = tmp;
		}
		return min;
	}
}

Mult(1,n);

// Complexité temps : n^n



int TabMult[n][n];

int MultMem(int i, int j) {
	int min, tmp;
	if (TabMult[i][j] == -1) {
		if (i==j) TabMult[i][j] = 0;
		else {
			min = MultMem(i,i) + MultMem(i+1,j) + d[i]*d[i+1]*d[j+1];
			for (int k = i+1; k<j; k++) {
				tmp = MultMem(i,k) + MultMem(k+1,j) + d[i]*d[k+1]*d[j+1];
				if (tmp < min) min = tmp;
			}
			TabMult[i][j] = min;
		}
	}
	return TabMult[i][j];
}

// Complexité mémoire : n^2
// Complexité temps : n^3

procedure MultOpt(D[n+1], S[n][n], TabMult[n][n]) {
	pour i=1 à n faire
		TabMult[i][i] = 0;
	pour t=1 à n-1 faire
		pour i=1 à n-t faire
			j = i+t;
			TabMult[i][j] = inf;
			pour k = i à j-1 faire
				tmp = TabMult[i][k] + TabMult[k+1][j] + D[i]D[k+1]D[j+1];
				si (tmp < TabMult[i][j]) alors
					TabMult[i][j] = tmp;
					S[i][j] = k;
}
