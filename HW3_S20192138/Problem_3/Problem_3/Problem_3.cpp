//
// 2021.11.16
// Problem_3 : Huffman coding 
// 20192138 조명재
//

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node {
	char symbol;
	int freq, order;
	struct _node* left;
	struct _node* right;
} NODE;

typedef struct _encode {
	char symbol;
	int freq;
	char* encoding;
} ENCODE;

NODE* u, * v, * w;
NODE* node[128];
ENCODE encode[128];

char buffer[1 << 20];
char symbolSet[128];
int freqSet[128];
int node_cnt;
int order;

void PQ_insert(NODE* tmp)
{
	int x;

	x = ++node_cnt;
	while (x != 1) {
		if (tmp->freq < node[x / 2]->freq) {
			node[x] = node[x / 2];
			x /= 2;
		}
		else if (tmp->freq == node[x / 2]->freq) {
			if (tmp->order < node[x / 2]->order) {
				node[x] = node[x / 2];
				x /= 2;
			}
			else break;
		}
		else break;
	}
	node[x] = tmp;
}

NODE* PQ_delete()
{
	int parent, child;
	NODE* item, * temp;

	item = node[1], temp = node[node_cnt--];
	parent = 1, child = 2;
	while (child <= node_cnt) {
		if (child < node_cnt) {
			if(node[child]->freq > node[child + 1]->freq) child++;
			else if (node[child]->freq == node[child + 1]->freq) {
				if (node[child]->order > node[child + 1]->order) child++;
			}
		}
		if (temp->freq < node[child]->freq) break;
		if (temp->freq > node[child]->freq) {
			node[parent] = node[child];
			parent = child;
			child *= 2;
		}
		else if (temp->freq == node[child]->freq) {
			if (temp->order > node[child]->order) {
				node[parent] = node[child];
				parent = child;
				child *= 2;
			}
			else break;
		}
	}

	node[parent] = temp;
	return item;
}

NODE* make_a_new_node()
{
	return (NODE*)malloc(sizeof(NODE));
}

void Set_Single_Node(char s, int f)
{
	int x;
	NODE* tmp;

	tmp = make_a_new_node();
	tmp->symbol = s;
	tmp->freq = f;
	tmp->left = tmp->right = nullptr;
	tmp->order = (int)s;

	PQ_insert(tmp);
}

void Make_Encoding(NODE* tmp, int depth, char* encoding) {
	if (tmp) {
		encoding[depth] = '0';
		Make_Encoding(tmp->left, depth + 1, encoding);
		if (tmp->left == nullptr && tmp->right == nullptr) {
			encoding[depth] = '\0';
			encode[(int)(tmp->symbol)].freq = tmp->freq;
			encode[(int)(tmp->symbol)].symbol = tmp->symbol;
			encode[(int)(tmp->symbol)].encoding = (char*)malloc(depth);
			for (int i = 0; i <= depth; i++)
				encode[(int)(tmp->symbol)].encoding[i] = encoding[i];
			return;
		}
		encoding[depth] = '1';
		Make_Encoding(tmp->right, depth + 1, encoding);
	}
}

void Find_Depth(NODE* tmp, int depth, int& max_depth)
{
	if (tmp) {
		Find_Depth(tmp->left, depth + 1, max_depth);
		if (tmp->left == nullptr && tmp->right == nullptr)
			max_depth = max_depth < depth ? depth : max_depth;
		Find_Depth(tmp->right, depth + 1, max_depth);
	}
}

char pack(const char* buf) 
{
	char res = 0;

	for (int i = 7; i >= 0; i--) {
		if (*buf == '\0') break;
		res |= (*buf - '0') << i;
		buf++;
	}

	return res;
}

void Huffman_Coding()
{
	FILE* in_fp, * out_fp, *bin_fp;
	int i, j, k, tree_depth, n, total_bit, total_freq, total, packing;
	char* encoding;
	char* pack_encoding;

	out_fp = fopen("P3_output_codewords.txt", "w");
	bin_fp = fopen("P3_output_encoded.bin", "wb");
	total_freq = n = 0;
	for (i = 0; i < 128; i++) {
		if (freqSet[i]) {
			Set_Single_Node(symbolSet[i], freqSet[i]);
			total_freq += freqSet[i];
			n++;
		}
	}

	for (i = 1; i <= n - 1; i++) {
		u = PQ_delete();
		v = PQ_delete();
		w = make_a_new_node();
	
		w->left = u;
		w->right = v;
		w->freq = u->freq + v->freq;
		w->symbol = NULL;
		w->order = --order;

		PQ_insert(w);
	}
	w = PQ_delete();

	tree_depth = 0;
	Find_Depth(w, 0, tree_depth);

	encoding = (char*)malloc((tree_depth + 1));
	Make_Encoding(w, 0, encoding);
	
	total_bit = total = 0;
	for (int i = 0; i < 128; i++) {
		if (freqSet[i]) {
			fprintf(out_fp, "%c %s %.2f\n", encode[i].symbol, encode[i].encoding,
				((float)encode[i].freq / (float)total_freq) * (float)100);
			total_bit += freqSet[i] * strlen(encode[i].encoding);
		}
	}
	total = total_bit / 8 + (total_bit % 8 != 0 ? 1 : 0);

	fwrite(&total_bit, sizeof(unsigned int), 1, bin_fp);
	fwrite(&total, sizeof(unsigned int), 1, bin_fp);

	in_fp = fopen("P3_input_ASCII.txt", "r");
	pack_encoding = (char*)malloc(total_bit + 1);
	k = 0;
	while (!feof(in_fp)) {
		fgets(buffer, sizeof(buffer), in_fp);
		i = 0;
		while (buffer[i] != '\0') {
			for (j = 0; j < strlen(encode[(int)buffer[i]].encoding); j++) {
				pack_encoding[k++] = encode[(int)buffer[i]].encoding[j];
			}
			i++;
		}
	}
	pack_encoding[k] = '\0';

	for (i = 0; i < total_bit; i += 8) {
		packing = pack(pack_encoding + i);
		fwrite(&packing, sizeof(char), 1, bin_fp);
	}

	fclose(in_fp);
	fclose(bin_fp);
	fclose(out_fp);
}

void check() // 결과 확인 함수
{
	int m;
	FILE* fp;
	fp = fopen("P3_output_encoded.bin", "rb");

	while ((m = fgetc(fp)) != EOF) {
		printf("%02X ", m);
	}

	fclose(fp);
}

int main(void)
{
	int i;
	char rFile[30], wFile[30];
	FILE* in_fp;

	in_fp = fopen("P3_input_ASCII.txt", "r");
	while (!feof(in_fp)) {
		fgets(buffer, sizeof(buffer), in_fp);
		i = 0;
		while (buffer[i] != '\0') {
			freqSet[(int)(buffer[i])]++;
			symbolSet[(int)(buffer[i])] = buffer[i];
			i++;
		}
	}
	fclose(in_fp);
	Huffman_Coding();

	//check(); // <= 결과 확인 용도

	return 0;
}