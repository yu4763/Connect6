#define stoneNum 19
#include "function.h"

//pos2 = -1; return 1: pos1 exist
//return 2: pos1 pos2 exist
//return 0: deosn't exist
int checkmine(int s[19][19], char state, int& pos1, int& pos2, int p, int q) {

	int i, j, k;

	/* check row */
	j = -1;
	int empty = 0;
	int emindex = -1;
	bool left = false;
	int cnt = 0;

	for (k = 0; k < stoneNum - 3; k++) {

		if (k < j) {
			continue;
		}

		if (s[p][k] == 0) {

			empty = 1;
			emindex = k;

			if (cnt > 0) {
				left = true;
			}
			else {
				left = false;
			}

			for (j = k + 1; ; j++) {
				if (j == stoneNum - 1)
					break;
				if (s[p][j] != 0) {
					break;
				}
				if (empty < 2) {
					empty++;
				}
				emindex = j;
			}
		}

		else if (s[p][k] == state) {
			cnt = 1;
			for (j = k + 1; j < k + 6; j++) {
				if (j >= stoneNum)
					break;
				if (s[p][j] != state) {
					break;
				}
				cnt++;
			}

			if (cnt == 5) {
				if (empty > 1 || (empty == 1 && !left)) {
					pos1 = p * stoneNum + emindex;
					pos2 = -1;
					return 1;
				}
				else if (!(j == stoneNum) && s[p][j] == 0) {
					if (j + 1 == stoneNum || s[p][j + 1] != state) {
						pos1 = p * stoneNum + j;
						pos2 = -1;
						return 1;
					}
				}
			}

			else if (cnt == 4) {
				if (empty > 2 || (empty == 2 && !left)) {
					pos1 = p * stoneNum + emindex;
					pos2 = p * stoneNum + (emindex - 1);
					return 2;
				}
				else if (empty == 1 && !left) {
					if ((j <= stoneNum - 1) && s[p][j] == 0) {
						if (j + 1 == stoneNum || s[p][j + 1] != state) {
							pos1 = p * stoneNum + emindex;
							pos2 = p * stoneNum + j;
							return 2;
						}
					}
				}
				else if ((j <= stoneNum - 2) && s[p][j] == 0 && s[p][j + 1] == 0) {
					if (j + 2 == stoneNum || s[p][j + 2] != state) {
						pos1 = p * stoneNum + j;
						pos2 = p * stoneNum + j + 1;
						return 2;
					}
				}
			}
		}

		else {
			empty = 0;
			cnt = 0;
		}

	}

	// /*check column*/
	j = -1;
	empty = 0;
	emindex = -1;
	left = false;
	cnt = 0;

	for (k = 0; k < stoneNum - 3; k++) {

		if (k < j) {
			continue;
		}

		if (s[k][q] == 0) {

			empty = 1;
			emindex = k;

			if (cnt > 0) {
				left = true;
			}
			else {
				left = false;
			}

			for (j = k + 1; ; j++) {
				if (j == stoneNum - 1)
					break;
				if (s[j][q] != 0) {
					break;
				}
				if (empty < 2) {
					empty++;
				}
				emindex = j;
			}
		}

		else if (s[k][q] == state) {
			cnt = 1;
			for (j = k + 1; j < k + 6; j++) {
				if (j >= stoneNum)
					break;
				if (s[j][q] != state) {
					break;
				}
				cnt++;
			}

			if (cnt == 5) {
				if (empty > 1 || (empty == 1 && !left)) {
					pos1 = emindex * stoneNum + q;
					pos2 = -1;
					return 1;
				}
				else if (!(j == stoneNum) && s[j][q] == 0) {
					if (j + 1 == stoneNum || s[j + 1][q] != state) {
						pos1 = j * stoneNum + q;
						pos2 = -1;
						return 1;
					}
				}
			}

			else if (cnt == 4) {
				if (empty > 2 || (empty == 2 && !left)) {
					pos1 = emindex * stoneNum + q;
					pos2 = (emindex - 1)*stoneNum + q;
					return 2;
				}
				else if (empty == 1 && !left) {
					if ((j <= stoneNum - 1) && s[j][q] == 0) {
						if (j + 1 == stoneNum || s[j + 1][q] != state) {
							pos1 = emindex * stoneNum + q;
							pos2 = j * stoneNum + q;
							return 2;
						}
					}
				}
				else if ((j <= stoneNum - 2) && s[j][q] == 0 && s[j + 1][q] == 0) {
					if (j + 2 == stoneNum || s[j + 2][q] != state) {
						pos1 = j * stoneNum + q;
						pos2 = (j + 1)*stoneNum + q;
						return 2;
					}
				}
			}
		}

		else {
			empty = 0;
			cnt = 0;
		}

	}

	/* check diagonal*/

	for (i = 0, k = 0; k < stoneNum - 5; k++) {

		empty = 0;
		emindex = -1;
		left = false;
		cnt = 0;
		int t = -1;

		for (j = 0; ; j++) {

			if (i + j >= stoneNum || k + j >= stoneNum)
				break;

			if (j < t)
				continue;

			if (s[i + j][k + j] == 0) {

				empty = 1;
				emindex = j;

				if (cnt > 0) {
					left = true;
				}
				else {
					left = false;
				}

				for (t = j + 1; ; t++) {
					if (i + t >= stoneNum - 1 || k + t >= stoneNum - 1)
						break;
					if (s[i + t][k + t] != 0) {
						break;
					}
					if (empty < 2) {
						empty++;
					}
					emindex = t;
				}

			}

			else if (s[i + j][k + j] == state) {
				cnt = 1;
				for (t = j + 1; t < j + 6; t++) {
					if (i + t >= stoneNum || k + t >= stoneNum)
						break;
					if (s[i + t][k + t] != state) {
						break;
					}
					cnt++;
				}

				if (cnt == 5) {
					if (empty > 1 || (empty == 1 && !left)) {
						pos1 = (i + emindex)*stoneNum + k + emindex;
						pos2 = -1;
						return 1;
					}
					else if ((i + t < stoneNum) && (k + t < stoneNum) && s[i + t][k + t] == 0) {
						if (i + t + 1 == stoneNum || k + t + 1 == stoneNum || s[i + t + 1][k + t + 1] != state) {
							pos1 = (i + t)*stoneNum + (k + t);
							pos2 = -1;
							return 1;
						}
					}
				}

				else if (cnt == 4) {
					if (empty > 2 || (empty == 2 && !left)) {
						pos1 = (i + emindex)*stoneNum + k + emindex;
						pos2 = (i + emindex - 1)*stoneNum + k + emindex - 1;
						return 2;
					}
					else if (empty == 1 && !left) {
						if ((i + t < stoneNum) && (k + t < stoneNum) && s[i + t][k + t] == 0) {
							if (i + t + 1 == stoneNum || k + t + 1 == stoneNum || s[i + t + 1][k + t + 1] != state) {
								pos1 = (i + emindex)*stoneNum + k + emindex;
								pos2 = (i + t)*stoneNum + (k + t);
								return 2;
							}
						}
					}
					else if ((i + t <= stoneNum - 2) && (k + t <= stoneNum - 2) && s[i + t][k + t] == 0 && s[i + t + 1][k + t + 1] == 0) {
						if (i + t + 2 == stoneNum || k + t + 2 == stoneNum || s[i + t + 2][k + t + 2] != state) {
							pos1 = (i + t)*stoneNum + k + t;
							pos2 = (i + t + 1)*stoneNum + k + t + 1;
							return 2;
						}
					}
				}
			}

			else {
				empty = 0;
				cnt = 0;
			}


		}


	}

	for (i = 0, k = 0; i < stoneNum - 5; i++) {

		empty = 0;
		emindex = -1;
		left = false;
		cnt = 0;
		int t = -1;

		for (j = 0; ; j++) {

			if (i + j >= stoneNum || k + j >= stoneNum)
				break;

			if (j < t)
				continue;

			if (s[i + j][k + j] == 0) {

				empty = 1;
				emindex = j;

				if (cnt > 0) {
					left = true;
				}
				else {
					left = false;
				}

				for (t = j + 1; ; t++) {
					if (i + t >= stoneNum - 1 || k + t >= stoneNum - 1)
						break;
					if (s[i + t][k + t] != 0) {
						break;
					}
					if (empty < 2) {
						empty++;
					}
					emindex = t;
				}

			}

			else if (s[i + j][k + j] == state) {
				cnt = 1;
				for (t = j + 1; t < j + 6; t++) {
					if (i + t >= stoneNum || k + t >= stoneNum)
						break;
					if (s[i + t][k + t] != state) {
						break;
					}
					cnt++;
				}

				if (cnt == 5) {
					if (empty > 1 || (empty == 1 && !left)) {
						pos1 = (i + emindex)*stoneNum + k + emindex;
						pos2 = -1;
						return 1;
					}
					else if ((i + t < stoneNum) && (k + t < stoneNum) && s[i + t][k + t] == 0) {
						if (i + t + 1 == stoneNum || k + t + 1 == stoneNum || s[i + t + 1][k + t + 1] != state) {
							pos1 = (i + t)*stoneNum + (k + t);
							pos2 = -1;
							return 1;
						}
					}
				}

				else if (cnt == 4) {
					if (empty > 2 || (empty == 2 && !left)) {
						pos1 = (i + emindex)*stoneNum + k + emindex;
						pos2 = (i + emindex - 1)*stoneNum + k + emindex - 1;
						return 2;
					}
					else if (empty == 1 && !left) {
						if ((i + t < stoneNum) && (k + t < stoneNum) && s[i + t][k + t] == 0) {
							if (i + t + 1 == stoneNum || k + t + 1 == stoneNum || s[i + t + 1][k + t + 1] != state) {
								pos1 = (i + emindex)*stoneNum + k + emindex;
								pos2 = (i + t)*stoneNum + (k + t);
								return 2;
							}
						}
					}
					else if ((i + t <= stoneNum - 2) && (k + t <= stoneNum - 2) && s[i + t][k + t] == 0 && s[i + t + 1][k + t + 1] == 0) {
						if (i + t + 2 == stoneNum || k + t + 2 == stoneNum || s[i + t + 2][k + t + 2] != state) {
							pos1 = (i + t)*stoneNum + k + t;
							pos2 = (i + t + 1)*stoneNum + k + t + 1;
							return 2;
						}
					}
				}
			}

			else {
				empty = 0;
				cnt = 0;
			}


		}


	}


	/* check diagonal*/

	for (i = 0, k = 6; k < stoneNum; k++) {

		empty = 0;
		emindex = -1;
		left = false;
		cnt = 0;
		int t = -1;

		for (j = 0; ; j++) {

			if (i + j >= stoneNum || k - j < 0)
				break;

			if (j < t)
				continue;

			if (s[i + j][k - j] == 0) {

				empty = 1;
				emindex = j;

				if (cnt > 0) {
					left = true;
				}
				else {
					left = false;
				}

				for (t = j + 1; ; t++) {
					if (i + t >= stoneNum - 1 || k - t <= 0)
						break;
					if (s[i + t][k - t] != 0) {
						break;
					}
					if (empty < 2) {
						empty++;
					}
					emindex = t;
				}

			}

			else if (s[i + j][k - j] == state) {
				cnt = 1;
				for (t = j + 1; t < j + 6; t++) {
					if (i + t >= stoneNum || k - t <= 0)
						break;
					if (s[i + t][k - t] != state) {
						break;
					}
					cnt++;
				}


				if (cnt == 5) {
					if (empty > 1 || (empty == 1 && !left)) {
						pos1 = (i + emindex)*stoneNum + k - emindex;
						pos2 = -1;
						return 1;
					}
					else if ((i + t < stoneNum) && (k - t >= 0) && s[i + t][k - t] == 0) {
						if (i + t + 1 == stoneNum || k - t - 1 < 0 || s[i + t + 1][k - t - 1] != state) {
							pos1 = (i + t)*stoneNum + (k - t);
							pos2 = -1;
							return 1;
						}
					}
				}

				else if (cnt == 4) {
					if (empty > 2 || (empty == 2 && !left)) {
						pos1 = (i + emindex)*stoneNum + k - emindex;
						pos2 = (i + emindex - 1)*stoneNum + k - emindex + 1;
						return 2;
					}
					else if (empty == 1 && !left) {
						if ((i + t < stoneNum) && (k - t >= 0) && s[i + t][k - t] == 0) {
							if (i + t + 1 == stoneNum || k - t - 1 == stoneNum || s[i + t + 1][k - t - 1] != state) {
								pos1 = (i + emindex)*stoneNum + k - emindex;
								pos2 = (i + t)*stoneNum + (k - t);
								return 2;
							}
						}
					}

					////check here!!
					else if ((i + t <= stoneNum - 2) && (k - t >= 1) && s[i + t][k - t] == 0 && s[i + t + 1][k - t - 1] == 0) {
						if (i + t + 2 == stoneNum || k - t - 2 < 0 || s[i + t + 2][k - t - 2] != state) {
							pos1 = (i + t)*stoneNum + k - t;
							pos2 = (i + t + 1)*stoneNum + k - t - 1;
							return 2;
						}
					}
				}
			}

			else {
				empty = 0;
				cnt = 0;
			}


		}


	}

	for (k = stoneNum - 1, i = 0; i < stoneNum - 5; i++) {

		empty = 0;
		emindex = -1;
		left = false;
		cnt = 0;
		int t = -1;

		for (j = 0; ; j++) {

			if (i + j >= stoneNum || k - j < 0)
				break;

			if (j < t)
				continue;

			if (s[i + j][k - j] == 0) {

				empty = 1;
				emindex = j;

				if (cnt > 0) {
					left = true;
				}
				else {
					left = false;
				}

				for (t = j + 1; ; t++) {
					if (i + t >= stoneNum - 1 || k - t <= 0)
						break;
					if (s[i + t][k - t] != 0) {
						break;
					}
					if (empty < 2) {
						empty++;
					}
					emindex = t;
				}

			}

			else if (s[i + j][k - j] == state) {
				cnt = 1;
				for (t = j + 1; t < j + 6; t++) {
					if (i + t >= stoneNum || k - t <= 0)
						break;
					if (s[i + t][k - t] != state) {
						break;
					}
					cnt++;
				}


				if (cnt == 5) {
					if (empty > 1 || (empty == 1 && !left)) {
						pos1 = (i + emindex)*stoneNum + k - emindex;
						pos2 = -1;
						return 1;
					}
					else if ((i + t < stoneNum) && (k - t >= 0) && s[i + t][k - t] == 0) {
						if (i + t + 1 == stoneNum || k - t - 1 < 0 || s[i + t + 1][k - t - 1] != state) {
							pos1 = (i + t)*stoneNum + (k - t);
							pos2 = -1;
							return 1;
						}
					}
				}

				else if (cnt == 4) {
					if (empty > 2 || (empty == 2 && !left)) {
						pos1 = (i + emindex)*stoneNum + k - emindex;
						pos2 = (i + emindex - 1)*stoneNum + k - emindex + 1;
						return 2;
					}
					else if (empty == 1 && !left) {
						if ((i + t < stoneNum) && (k - t >= 0) && s[i + t][k - t] == 0) {
							if (i + t + 1 == stoneNum || k - t - 1 == stoneNum || s[i + t + 1][k - t - 1] != state) {
								pos1 = (i + emindex)*stoneNum + k - emindex;
								pos2 = (i + t)*stoneNum + (k - t);
								return 2;
							}
						}
					}

					////check here!!
					else if ((i + t <= stoneNum - 2) && (k - t >= 1) && s[i + t][k - t] == 0 && s[i + t + 1][k - t - 1] == 0) {
						if (i + t + 2 == stoneNum || k - t - 2 < 0 || s[i + t + 2][k - t - 2] != state) {
							pos1 = (i + t)*stoneNum + k - t;
							pos2 = (i + t + 1)*stoneNum + k - t - 1;
							return 2;
						}
					}
				}
			}

			else {
				empty = 0;
				cnt = 0;
			}


		}


	}


	checkmineblank(s, state, pos1, pos2, p, q);

}



int checkmineblank(int s[19][19], char state, int& pos1, int& pos2, int p, int q) {


	int cnt = 0;
	int empty = 0;
	int i, k;

	/*check row*/
	for (k = 0; k < stoneNum; k++) {

		if (s[p][k] == state) {
			cnt++;
		}
		else
			cnt = 0;

		if (cnt == 1) {

			if (k + 5 >= stoneNum)
				break;

			if (s[p][k + 1] == 0) {
				if (k + 6 >= stoneNum || s[p][k + 6] != state) {
					if (s[p][k + 2] == 0 && s[p][k + 3] == state && s[p][k + 4] == state && s[p][k + 5] == state) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = p * stoneNum + (k + 2);
						return 2;
					}
					else if (s[p][k + 2] == state && s[p][k + 3] == state && s[p][k + 4] == state && s[p][k + 5] == 0) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = p * stoneNum + (k + 5);
						return 2;
					}
					else if (s[p][k + 2] == state && s[p][k + 3] == state && s[p][k + 4] == state && s[p][k + 5] == state) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = -1;
						return 1;
					}
				}
			}

		}

		if (cnt == 2) {

			if (k + 4 >= stoneNum)
				break;

			if (s[p][k + 1] == 0) {

				if (k + 5 >= stoneNum || s[p][k + 5] != state) {
					if (s[p][k + 2] == 0 && s[p][k + 3] == state && s[p][k + 4] == state) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = p * stoneNum + (k + 2);
						return 2;
					}
					else if (s[p][k + 2] == state && s[p][k + 3] == state && s[p][k + 4] == 0) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = p * stoneNum + (k + 4);
						return 2;
					}
					else if (s[p][k + 2] == state && s[p][k + 3] == state && s[p][k + 4] == state) {
						pos1 = p * stoneNum + (k + 1);
						pos2 = -1;
						return 1;
					}

				}

			}

		}


		if (cnt == 3) {

			if (k + 3 >= stoneNum)
				break;

			else {

				if (s[p][k + 1] == 0) {

					if (s[p][k + 2] == 0 && s[p][k + 3] == state) {

						if (k + 4 >= stoneNum || s[p][k + 4] != state) {
							pos1 = p * stoneNum + (k + 1);
							pos2 = p * stoneNum + (k + 2);
							return 2;
						}
					}
					else if (s[p][k + 2] == state) {

						if (k + 4 >= stoneNum || s[p][k + 4] != state) {
							if (s[p][k + 3] == state) {
								pos1 = p * stoneNum + (k + 1);
								pos2 = -1;
								return 1;
							}
							else if (s[p][k + 3] == 0) {

								pos1 = p * stoneNum + (k + 1);
								pos2 = p * stoneNum + (k + 3);
								return 2;

							}
						}

					}

				}
				else if (s[p][k + 1] == state) {

					if (s[p][k + 2] == 0 && s[p][k + 3] == state) {
						if (k + 4 >= stoneNum || s[p][k + 4] != state) {
							pos1 = p * stoneNum + (k + 2);
							pos2 = -1;
							return 1;
						}
					}

				}

			}

		}
	}

	for (k = 0; k < stoneNum; k++) {

		if (s[p][k] == 0)
			empty++;
		else if (s[p][k] == state) {
			if (empty > 1 || (empty == 1 && k - 2 < 0)) {
				if (k + 4 >= stoneNum)
					break;

				if (k + 5 >= stoneNum || s[p][k + 5] != state) {
					if (s[p][k + 1] == state && s[p][k + 2] == 0 && s[p][k + 3] == state && s[p][k + 4] == state) {
						pos1 = p * stoneNum + (k - 1);
						pos2 = p * stoneNum + (k + 2);
						return 2;
					}
					else if (s[p][k + 1] == state && s[p][k + 2] == state && s[p][k + 3] == 0 && s[p][k + 4] == state) {
						pos1 = p * stoneNum + (k - 1);
						pos2 = p * stoneNum + (k + 3);
						return 2;
					}

					else if (s[p][k + 1] == 0 && s[p][k + 2] == state && s[p][k + 3] == state && s[p][k + 4] == state) {
						pos1 = p * stoneNum + (k - 1);
						pos2 = p * stoneNum + (k + 1);
						return 2;
					}

				}

			}

			empty = 0;
		}
		else
			empty = 0;

	}

	/* check column */

	cnt = 0;
	empty = 0;

	for (i = 0; i < stoneNum; i++) {

		if (s[i][q] == state) {
			cnt++;
		}
		else
			cnt = 0;

		if (cnt == 1) {

			if (i + 5 >= stoneNum)
				break;

			if (s[i + 1][q] == 0) {
				if (i + 6 >= stoneNum || s[i + 6][q] != state) {
					if (s[i + 2][q] == 0 && s[i + 3][q] == state && s[i + 4][q] == state && s[i + 5][q] == state) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = (i + 2)*stoneNum + q;
						return 2;
					}
					else if (s[i + 2][q] == state && s[i + 3][q] == state && s[i + 4][q] == state && s[i + 5][q] == 0) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = (i + 5)*stoneNum + q;
						return 2;
					}
					else if (s[i + 2][q] == state && s[i + 3][q] == state && s[i + 4][q] == state && s[i + 5][q] == state) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = -1;
						return 1;
					}
				}
			}

		}

		if (cnt == 2) {

			if (i + 4 >= stoneNum)
				break;

			if (s[i + 1][q] == 0) {

				if (i + 5 >= stoneNum || s[i + 5][q] != state) {
					if (s[i + 2][q] == 0 && s[i + 3][q] == state && s[i + 4][q] == state) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = (i + 2)*stoneNum + q;
						return 2;
					}
					else if (s[i + 2][q] == state && s[i + 3][q] == state && s[i + 4][q] == 0) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = (i + 4)*stoneNum + q;
						return 2;
					}
					else if (s[i + 2][q] == state && s[i + 3][q] == state && s[i + 4][q] == state) {
						pos1 = (i + 1)*stoneNum + q;
						pos2 = -1;
						return 1;
					}

				}

			}

		}


		if (cnt == 3) {

			if (i + 3 >= stoneNum)
				break;

			else {

				if (s[i + 1][q] == 0) {

					if (s[i + 2][q] == 0 && s[i + 3][q] == state) {

						if (i + 4 >= stoneNum || s[i + 4][q] != state) {
							pos1 = (i + 1)*stoneNum + q;
							pos2 = (i + 2)*stoneNum + q;
							return 2;
						}
					}
					else if (s[i + 2][q] == state) {

						if (i + 4 >= stoneNum || s[i + 4][q] != state) {
							if (s[i + 3][q] == state) {
								pos1 = (i + 1)*stoneNum + q;
								pos2 = -1;
								return 1;
							}
							else if (s[i + 3][q] == 0) {

								pos1 = (i + 1)*stoneNum + q;
								pos2 = (i + 3)*stoneNum + q;
								return 2;

							}
						}

					}

				}
				else if (s[i + 1][q] == state) {

					if (s[i + 2][q] == 0 && s[i + 3][q] == state) {
						if (i + 4 >= stoneNum || s[i + 4][q] != state) {
							pos1 = (i + 2)*stoneNum + q;
							pos2 = -1;
							return 1;
						}
					}

				}

			}

		}
	}

	cnt = 0;
	empty = 0;

	for (i = 0; i < stoneNum; i++) {

		if (s[i][q] == 0)
			empty++;
		else if (s[i][q] == state) {
			if (empty > 1 || (empty == 1 && i - 2 < 0)) {
				if (i + 4 >= stoneNum)
					break;

				if (i + 5 >= stoneNum || s[i + 5][q] != state) {
					if (s[i + 1][q] == state && s[i + 2][q] == 0 && s[i + 3][q] == state && s[i + 4][q] == state) {
						pos1 = (i - 1)*stoneNum + q;
						pos2 = (i + 2)*stoneNum + q;
						return 2;
					}
					else if (s[i + 1][q] == state && s[i + 2][q] == state && s[i + 3][q] == 0 && s[i + 4][q] == state) {
						pos1 = (i - 1)*stoneNum + q;
						pos2 = (i + 3)*stoneNum + q;
						return 2;
					}

					else if (s[i + 1][q] == 0 && s[i + 2][q] == state && s[i + 3][q] == state && s[i + 4][q] == state) {
						pos1 = (i - 1)*stoneNum + q;
						pos2 = (i + 1)*stoneNum + q;
						return 2;
					}

				}

			}

			empty = 0;
		}
		else
			empty = 0;

	}

	/* check diagonal */

	int j;
	int max, min;

	if (p <= q) {
		min = p;
		max = q;
	}
	else {
		min = q;
		max = p;
	}

	cnt = 0;
	empty = 0;

	for (j = -min; j < stoneNum - max; j++) {

		if (s[p + j][q + j] == state) {
			cnt++;
		}
		else
			cnt = 0;

		if (cnt == 1) {

			if (j + 5 >= stoneNum - max)
				break;

			if (s[p + j + 1][q + j + 1] == 0) {
				if (j + 6 >= stoneNum - max || s[p + j + 6][q + j + 6] != state) {
					if (s[p + j + 2][q + j + 2] == 0 && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state && s[p + j + 5][q + j + 5] == state) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = (p + j + 2)*stoneNum + q + j + 2;
						return 2;
					}
					else if (s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state && s[p + j + 5][q + j + 5] == 0) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = (p + j + 5)*stoneNum + q + j + 5;
						return 2;
					}
					else if (s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state && s[p + j + 5][q + j + 5] == state) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = -1;
						return 1;
					}
				}
			}

		}

		else if (cnt == 2) {
			if (j + 4 >= stoneNum - max)
				break;

			if (s[p + j + 1][q + j + 1] == 0) {

				if (j + 5 >= stoneNum - max || s[p + j + 5][q + j + 5] != state) {
					if (s[p + j + 2][q + j + 2] == 0 && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = (p + j + 2)*stoneNum + q + j + 2;
						return 2;
					}
					else if (s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == 0) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = (p + j + 4)*stoneNum + q + j + 4;
						return 2;
					}
					else if (s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state) {
						pos1 = (p + j + 1)*stoneNum + q + j + 1;
						pos2 = -1;
						return 1;
					}

				}

			}

		}


		else if (cnt == 3) {
			if (j + 3 >= stoneNum - max)
				break;


			else {

				if (s[p + j + 1][q + j + 1] == 0) {


					if (s[p + j + 2][q + j + 2] == 0 && s[p + j + 3][q + j + 3] == state) {

						if (j + 4 >= stoneNum - max || s[p + j + 4][q + j + 4] != state) {
							pos1 = (p + j + 1)*stoneNum + q + j + 1;
							pos2 = (p + j + 2)*stoneNum + q + j + 2;
							return 2;
						}
					}
					else if (s[p + j + 2][q + j + 2] == state) {

						if (j + 4 >= stoneNum - max || s[p + j + 4][q + j + 4] != state) {
							;
							if (s[p + j + 3][q + j + 3] == state) {
								pos1 = (p + j + 1)*stoneNum + q + j + 1;
								pos2 = -1;
								return 1;
							}
							else if (s[p + j + 3][q + j + 3] == 0) {

								pos1 = (p + j + 1)*stoneNum + q + j + 1;
								pos2 = (p + j + 3)*stoneNum + q + j + 3;
								return 2;

							}
						}

					}

				}
				else if (s[p + j + 1][q + j + 1] == state) {

					if (s[p + j + 2][q + j + 2] == 0 && s[p + j + 3][q + j + 3] == state) {
						if (j + 4 >= stoneNum - max || s[p + j + 4][q + j + 4] != state) {
							pos1 = (p + j + 2)*stoneNum + q + j + 2;
							pos2 = -1;
							return 1;
						}
					}

				}

			}

		}
	}

	cnt = 0;
	empty = 0;

	for (j = -min; j < stoneNum - max; j++) {

		if (s[p + j][q + j] == 0)
			empty++;
		else if (s[p + j][q + j] == state) {
			if (empty > 1 || (empty == 1 && j - 2 < -min)) {
				if (j + 4 >= stoneNum - max)
					break;

				if (j + 5 >= stoneNum - max || s[p + j + 5][q + j + 5] != state) {
					if (s[p + j + 1][q + j + 1] == state && s[p + j + 2][q + j + 2] == 0 && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q + j - 1;
						pos2 = (p + j + 2)*stoneNum + q + j + 2;
						return 2;
					}
					else if (s[p + j + 1][q + j + 1] == state && s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == 0 && s[p + j + 4][q + j + 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q + j - 1;
						pos2 = (p + j + 3)*stoneNum + q + j + 3;
						return 2;
					}

					else if (s[p + j + 1][q + j + 1] == 0 && s[p + j + 2][q + j + 2] == state && s[p + j + 3][q + j + 3] == state && s[p + j + 4][q + j + 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q + j - 1;
						pos2 = (p + j + 1)*stoneNum + q + j + 1;
						return 2;
					}

				}

			}

			empty = 0;
		}
		else
			empty = 0;

	}

	/*check diagonal */
	if (p - 0 < stoneNum - 1 - q) {
		min = p;
	}
	else {
		min = stoneNum - 1 - q;
	}

	if (q - 0 < stoneNum - 1 - p) {
		max = q;
	}
	else {
		max = stoneNum - 1 - p;
	}

	cnt = 0;
	empty = 0;

	for (j = -min; j <= max; j++) {

		if (s[p + j][q - j] == state) {
			cnt++;
		}
		else
			cnt = 0;

		if (cnt == 1) {

			if (j + 5 > max)
				break;

			if (s[p + j + 1][q - j - 1] == 0) {
				if (j + 6 > max || s[p + j + 6][q - j - 6] != state) {
					if (s[p + j + 2][q - j - 2] == 0 && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state && s[p + j + 5][q - j - 5] == state) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = (p + j + 2)*stoneNum + q - j - 2;
						return 2;
					}
					else if (s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state && s[p + j + 5][q - j - 5] == 0) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = (p + j + 5)*stoneNum + q - j - 5;
						return 2;
					}
					else if (s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state && s[p + j + 5][q - j - 5] == state) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = -1;
						return 1;
					}
				}
			}

		}

		if (cnt == 2) {
			if (j + 4 > max)
				break;

			if (s[p + j + 1][q - j - 1] == 0) {

				if (j + 5 > max || s[p + j + 5][q - j - 5] != state) {
					if (s[p + j + 2][q - j - 2] == 0 && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = (p + j + 2)*stoneNum + q - j - 2;
						return 2;
					}
					else if (s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == 0) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = (p + j + 4)*stoneNum + q - j - 4;
						return 2;
					}
					else if (s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state) {
						pos1 = (p + j + 1)*stoneNum + q - j - 1;
						pos2 = -1;
						return 1;
					}

				}

			}

		}


		if (cnt == 3) {

			if (j + 3 > max)
				break;

			else {

				if (s[p + j + 1][q - j - 1] == 0) {

					if (s[p + j + 2][q - j - 2] == 0 && s[p + j + 3][q - j - 3] == state) {

						if (j + 4 > max || s[p + j + 4][q - j - 4] != state) {
							pos1 = (p + j + 1)*stoneNum + q - j - 1;
							pos2 = (p + j + 2)*stoneNum + q - j - 2;
							return 2;
						}
					}
					else if (s[p + j + 2][q - j - 2] == state) {

						if (j + 4 > max || s[p + j + 4][q - j - 4] != state) {
							if (s[p + j + 3][q - j - 3] == state) {
								pos1 = (p + j + 1)*stoneNum + q - j - 1;
								pos2 = -1;
								return 1;
							}
							else if (s[p + j + 3][q - j - 3] == 0) {

								pos1 = (p + j + 1)*stoneNum + q - j - 1;
								pos2 = (p + j + 3)*stoneNum + q - j - 3;
								return 2;

							}
						}

					}

				}
				else if (s[p + j + 1][q - j - 1] == state) {

					if (s[p + j + 2][q - j - 2] == 0 && s[p + j + 3][q - j - 3] == state) {
						if (j + 4 > max || s[p + j + 4][q - j - 4] != state) {
							pos1 = (p + j + 2)*stoneNum + q - j - 2;
							pos2 = -1;
							return 1;
						}
					}

				}

			}

		}
	}

	cnt = 0;
	empty = 0;

	for (j = -min; j <= max; j++) {

		if (s[p + j][q - j] == 0)
			empty++;
		else if (s[p + j][q - j] == state) {
			if (empty > 1 || (empty == 1 && j - 2 < -min)) {
				if (j + 4 > max)
					break;

				if (j + 5 > max || s[p + j + 5][q - j - 5] != state) {
					if (s[p + j + 1][q - j - 1] == state && s[p + j + 2][q - j - 2] == 0 && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q - j + 1;
						pos2 = (p + j + 2)*stoneNum + q - j - 2;
						return 2;
					}
					else if (s[p + j + 1][q - j - 1] == state && s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == 0 && s[p + j + 4][q - j - 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q - j + 1;
						pos2 = (p + j + 3)*stoneNum + q - j - 3;
						return 2;
					}

					else if (s[p + j + 1][q - j - 1] == 0 && s[p + j + 2][q - j - 2] == state && s[p + j + 3][q - j - 3] == state && s[p + j + 4][q - j - 4] == state) {
						pos1 = (p + j - 1)*stoneNum + q - j + 1;
						pos2 = (p + j + 1)*stoneNum + q - j - 1;
						return 2;
					}

				}

			}

			empty = 0;
		}
		else
			empty = 0;

	}

	pos1 = -1;
	pos2 = -1;

	return 0;

}