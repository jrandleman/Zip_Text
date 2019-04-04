/* AUTHOR: JORDAN RANDLEMAN */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define CH_IS_GCAP(ch_c_inst) ((ch_c_inst) >= 'A' && (ch_c_inst) <= 'Z')
#define DASH_PUNC(C_CH) (C_CH=='['||C_CH=='{'||C_CH==']'||C_CH=='}'||C_CH==':'||C_CH==';'||C_CH=='('||C_CH==')'||C_CH=='/')
#define LESS_THAN_8_BITS_LEFT(BIT_TOT,BIT_ID) ((BIT_TOT - (8 * (BIT_ID + 1))) < 0)
#define REMAINING_BITS(BIT_TOT,BIT_ID) (8 - ((8 * (BIT_ID + 1)) - BIT_TOT))
/* 'HIDE' FILE FUNCTIONS */
void hide(char *, char *);
void write_pass_ss_keys(char [][152], int, char *);
void read_txt_engl(char *, char *);
void write_int_compressed(char *, char *);
/* 'SHOW' FILE FUNCTIONS */
void show(char *, char *);
void read_pass_ss_keys(char *);
void show_txt_compressed(char *, char *);
/* BOTH => ERROR/PASSWORD/TXT-FILE CONVERT FUNCTIONS */
void err_info();
void convert_password_to_txt(char *, char *);
void convert_txt_to_bin(char *, char *);
void convert_pass_to_bin(char *, char *);
/* COMMON WORD SUBSTITUTION FUNCTIONS */
void splice_str(char *, char *, int, int);
int delta_sub_words(char *, int, char [][50], char [][50]);
void print_cw_used(int);
/* CHARACTER/COMPRESSION FUNCTIONS */
void rm_nxt_ch(char *, int);
void modify_s(char *, int);
void s_decompress(char [][152], char *);
int s_compress(char [][152], char *);
void process_split_s(char [][152], char *, char *, char *);
/* SUBSTRING REFERENCE KEY HANDLER FUNCTIONS */
int is_ss_ref(int, char);
int ss_ch_index(char);
char adjust_ref_char(char);
/* SUBSTRING FUNCTIONS */
void store_ss(char [][152], char *, int);
int clean_ss(char [][152], char *, int);
void splice_ss(char [][152], int [40], int, int);
void trim_ss(char [][152], char *, int [40], int, int);
void print_ss(char [][152], int);
/* INT COMPRESSION FUNCTIONS */
int count_newl(char *);
int find_newl(char *);
void two_ptr_str_splice(char *, char *, char *);
int get_ch_shift(char);
int get_num_shift(int);
int str_to_int(char *);
int int_to_str(unsigned int, char *, int);
void hide_int(char *, char *, char *);
void show_int(char *, char *);
void process_split_int(char *);
/* DUPLICATE SHORT HANDLERS FUNCITONS */
int count_double_shorts(unsigned short[], unsigned short[], int);
int short_num_copies(unsigned short, unsigned short[], int);
/* CONVERTING 2 SHORTS TO 1 INT AND VISE-VERSA  */
unsigned int shorts_to_int(unsigned short, unsigned short);
void int_to_shorts(unsigned int, unsigned short *, unsigned short *);
int get_short_char_order(int[], unsigned short[], unsigned short[], int, int);
/* PACKING AND UNPACKING BITS INTO INTO SHORTS FUNCTIONS */
void fill_temp_with_buffer(int[], int, int[], int);
int get_shift_number(int);
void pack(int[], int, char[], int);
void unpack(int *, int, char);
void packer(int, int, int[], char[]);
void unpacker(int, int, int[], char[]); 
/* COMMON WORD SUBSTITUTIONS */
char cw_keys[222][50] = { /* single letter */
	"_b_", "_d_", "_e_", "_f_", "_g_", "_h_", "_j_", "_k_", "_l_", "_m_", "_n_", "_o_", 
	"_p_", "_q_", "_r_", "_s_", "_t_", "_u_", "_v_", "_w_", "_x_", "_y_", "_z_", 
	/* two-letter 1 */
	"_bb_", "_bc_", "_bd_", "_bf_", "_bg_", "_bh_", "_bk_", "_bl_", "_bm_", "_bn_", "_bp_", 
	"_bq_", "_br_", "_bs_", "_bt_", "_bv_", "_bw_", "_bx_", "_bz_", "_cb_", "_cc_", "_cd_", 
	"_cf_", "_cg_", "_ch_", "_ck_", "_cl_", "_cm_", "_cn_", "_cp_", "_cq_", "_cr_", "_cs_", 
	"_ct_", "_cv_", "_cw_", "_cx_", "_cz_", "_db_", "_dc_", "_dd_", "_df_", "_dg_", "_dh_", 
	"_dk_", "_dl_", "_dm_", "_dn_", "_dp_", "_dq_", "_dr_", "_ds_", "_dt_", "_dv_", "_dw_", 
	"_dx_", "_dz_", "_fb_", "_fc_", "_fd_", "_ff_", "_fg_", "_fh_", "_fk_", "_fl_", 
	"_fm_", "_fn_", 
	/* two-letter 2 */
	"_fp_", "_fq_", "_fr_", "_fs_", "_ft_", "_fv_", "_fw_", "_fx_", "_fz_", "_gb_", 
	"_gc_", "_gd_", "_gf_", "_gg_", "_gh_", "_gk_", "_gl_", "_gm_", "_gn_", "_gp_", 
	"_gq_", "_gr_", "_gs_", "_gt_", "_gv_", "_gw_", "_gx_", "_gz_", "_hb_", "_hc_", 
	"_hd_", "_hf_", "_hg_", "_hh_", "_hk_", "_hl_", "_hm_", "_hn_", "_hp_", 
	"_hq_", "_hr_", "_hs_", "_ht_", "_hv_", "_hw_", "_hx_", "_hz_", "_kb_", "_kc_", 
	"_kd_", "_kf_", 
	/* NO-SPACE */
	"`z", "`?", "`r", "`s", "`t", "`u", "`v", "`w", "`x", "`y", 
	"`k", "`l", "`m", "`n", "`o", "`p", "`q", 
	"`b", "`c", "`d", "`e", "`f", "`g", "`h", "`i", "`j", "!s", 
	"!t", "!u", "!v", "!w", "!x", "!y", "!z", "`a", "!j", "!k", 
	"!l", "!m", "!n", "!o", "!p", "!q", "!r", "!h", "!i", "`!", 
	"?w", "?x", "?y", "?z", "!a", "!b", "!c", "!d", "!e", "!f", "!g", 
	"?l", "?m", "?n", "?o", "?p", "?q", "?r", "?s", "?t", "?u", "?v", 
	"?a", "?b", "?c", "?d", "?e", "?f", "?g", "?h", "?i", "?j", "?k"
};
char cw_word[222][50] = { /* single letter */
	"_at_", "_as_", "_an_", "_be_", "_by_", "_do_", "_go_", "_in_", "_is_", "_it_", "_my_", "_of_", 
	"_on_", "_or_", "_to_", "_up_", "_and_", "_the_", "_have_", "_that_", "_this_", "_with_", "_you_", 
	/* two-letter 1 */
	"_any_", "_all_", "_but_", "_can_", "_day_", "_get_", "_him_", "_his_", "_her_", "_how_", "_now_", 
	"_new_", "_out_", "_one_", "_our_", "_two_", "_use_", "_way_", "_who_", "_its_", "_say_", "_see_", 
	"_she_", "_also_", "_come_", "_from_", "_for_", "_back_", "_give_", "_good_", "_just_", "_into_", "_know_", 
	"_look_", "_like_", "_make_", "_most_", "_them_", "_over_", "_only_", "_some_", "_time_", "_take_", "_they_", 
	"_work_", "_want_", "_will_", "_year_", "_your_", "_what_", "_when_", "_then_", "_than_", "_could_", "_well_", 
	"_because_", "_even_", "_these_", "_which_", "_people_", "_about_", "_after_", "_first_", "_other_", "_think_", 
	"_there_", "_their_", 
	/* two-letter 2 */
	"_ask_", "_big_", "_bad_", "_eye_", "_few_", "_man_", "_own_", "_old_", "_try_", "_able_", 
	"_case_", "_call_", "_fact_", "_find_", "_feel_", "_hand_", "_high_", "_life_", "_last_", "_long_", 
	"_next_", "_part_", "_seem_", "_same_", "_tell_", "_week_", "_child_", "_company_", "_different_", "_early_", 
	"_government_", "_group_", "_great_", "_important_", "_leave_", "_little_", "_large_", "_number_", "_not_", 
	"_person_", "_place_", "_point_", "_problem_", "_public_", "_right_", "_small_", "_thing_", "_world_", "_woman_", 
	"_young_", "_would_", 
	/* NO-SPACE */
	"person", "different", "after", "other", "there", "group", "place", "point", "right", "thing", 
	"hand", "high", "last", "long", "part", "tell", "tion", 
	"then", "than", "well", "even", "able", "case", "call", "fact", "feel", 
	"make", "most", "them", "over", "some", "time", "take", "work", "will", 
	"that", "this", "with", "come", "back", "ent", "into", "know", "nter", 
	"for", "ear", "ing", "end", "see", "she", "ask", "big", "bad", "eye", "man", 
	"own", "old", "try", "her", "how", "now", "new", "out", "one", "our", "use", 
	"way", "who", "its", "and", "not", "the", "you", "any", "all", "but", "can", 
	"day", "get", "his"
};
/* GLOBAL VARIABLES */
char ss_array_matrix[300][40][152], s_compress_storage[300][152], s_max_buffer[30000];
char ss_refs[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$<=>@[]^{|}~\0";
int cw_idxs[224], chunk_count = 0, original_bytes = 0, zip_info = 0; // NO NEED COMPR_BYTES

int main(int argc, char *argv[]) {
	if(argc < 4) {
		err_info();
		return 0;
	}
	char *action = argv[3], *show_info;
	modify_s(action, 0); /* lowify */
	if(argc == 5) {
		show_info = argv[4];
		modify_s(show_info, 0); /* lowify */
		if(strcmp(show_info, "info") == 0) zip_info = 1;
	}
	if(strcmp(action, "hide") == 0) {
		hide(argv[2], argv[1]);
	} else if(strcmp(action, "show") == 0) {
		show(argv[2], argv[1]);
	} else {
		err_info();
	}
	return 0;
}
/******************************************************************************
* HIDE / SHOW FUNCTIONS
******************************************************************************/
void hide(char *arg2, char *arg1) {
	read_txt_engl(arg2, arg1); /* process text file in 150 char chunks */
	write_int_compressed(arg2, arg1); /* write compressed string to text file */
	int choice;
	printf("DELETE ORIGINAL TEXT FILE? (enter 1 - yes, 0 - no):\n\n>>> ");
	scanf("%d", &choice);
	if(choice != 1 && choice != 0) {
		printf("\n -:- INVALID OPTION -:- FILE NOT DELTED -:- \n");
	} else if(choice == 1) {
		printf("\n -:- FILE DELTED -:- \n");
		remove(arg1);
	}
	printf("\n");
}
void show(char *arg2, char *arg1) {
	char filename[75];
	convert_txt_to_bin(filename, arg1);
	read_pass_ss_keys(arg2);
	show_txt_compressed(arg2, arg1);
	printf("\n==============================================================================");
	printf("\n>> %s ==DECOMPRESS=> %s\n", filename, arg1);
	printf("==============================================================================\n\n");
}
void err_info() {
	printf("\n==================== INVALID EXECUTION! ====================\n");
	printf("$ gcc -o fzip fzip.c\n$ ./fzip textfile.txt yourpassword hide/show\n");
	printf("============================================================");
	printf("\n=> FILE.TXT FORMAT: AVOID NUMBERS, NEWLINES, & UNDERSCORES!\n");
	printf("============================================================");
	printf("\n=> COMPRESSION INFORMATION: $ ... hide/show info\n");
	printf("============================================================\n\n");
}
/******************************************************************************
* TEXT FILE FUNCTIONS
******************************************************************************/
void show_txt_compressed(char *arg2, char *arg1) {
	show_int(arg2, arg1);
	FILE *fp;
	if((fp = fopen(arg1, "w")) == NULL) { /* PUT DECOMPRESSED TEXT */
		printf("\n\n(!!!) ERROR WRITING DECOMPRESSED TEXT FILE (!!!)\n");
		return;
	}
	fwrite(s_max_buffer, sizeof(char), strlen(s_max_buffer), fp);
	fclose(fp);
	return;
}
void read_txt_engl(char *arg2, char *arg1) {
	char ss[40][152], s_chunk[152];
	int ret;
	FILE *fp;
	if((fp = fopen(arg1, "r")) == NULL) {
		printf("\n\n(!!!) ERROR READING UNCOMPRESSED TEXT FILE (!!!)\n\n");
		exit(0);
	}
	while((ret = fread(s_chunk, sizeof(char), 150, fp)) > 0) {
		s_chunk[ret] = '\0';
		original_bytes += ret;
		process_split_s(ss, s_chunk, s_compress_storage[chunk_count], arg2);
		chunk_count++;
	}
	fclose(fp);
}
/******************************************************************************
* BINARY/PASSWORD FILE FUNCTIONS
******************************************************************************/
void write_int_compressed(char *arg2, char *arg1) {
	for(int i = 0; i < chunk_count; i++) { /* copy compressed text into one string */
		char *p = s_max_buffer;
		if(i != 0) while(*p != '\0') p++;
		sprintf(p, "%s\n", s_compress_storage[i]);
	}
	int count = 0;
	char s_max[strlen(s_max_buffer)];
	char *q = s_max, *r = s_max_buffer;
	while(*r != '\0') *q++ = *r++;
	*q = '\0'; /* isolate only letters from s_max_buffer */
	hide_int(arg2, arg1, s_max); /* compress to integers */
}
void read_pass_ss_keys(char *arg2) {
	char ss_buffer[100], ch_buffer, filename[75];
	convert_password_to_txt(filename, arg2);
	int n = 0, m = 0; /* m - 2D ss array (sentence), n - ss (word) => WRT ss_array_matrix[m][n][] */
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL) {
		printf("\n\n(!!!) ERROR READING PASSWORD BIN FILE (!!!)\n\n");
		exit(0);
	}
	while(fscanf(fp, "%s%c", ss_buffer, &ch_buffer) > 0) {
		char *p = ss_buffer, *q = ss_array_matrix[m][n];
		while(*p != '\0') *q++ = *p++; /* copy ss from bin to 2d ss array in 3d matrix */
		*q = '\0';  /* terminate ss */
		if(ch_buffer == '\n') {
			ss_array_matrix[m][n+1][0] = '\0'; /* terminate ss 2d array */
			m++; /* move to next 2d ss array (1 per s_chunk) in 3d matrix */
			n = 0; /* start from row (ss) 0 */
		} else {
			n++; /* move to next row (ss) */
		}
	}
	fclose(fp);
	remove(filename); /* delete password txt file once ss keys retrieved */
}
void write_pass_ss_keys(char ss[][152], int ss_total, char *arg2) {
	char filename[75];
	convert_password_to_txt(filename, arg2);
	FILE *fp;
	(chunk_count == 0) ? (fp = fopen(filename, "w")) : (fp = fopen(filename, "a"));
	if(fp == NULL) {
		printf("\n\n(!!!) ERROR WRITING PASSWORD BIN FILE (!!!)\n");
		return;
	}
	for(int i = 0; i < ss_total; i++) {
		fprintf(fp, "%s", ss[i]);
		if(i < ss_total - 1) fprintf(fp, "%c", '\t');
	}
	fprintf(fp, "%c", '\n');
	fclose(fp);
}
void convert_password_to_txt(char *tfile, char *pass) {
	char *ptr = tfile, *qtr = pass; /* convert password into text file name */
	while(*qtr != '\0') *ptr++ = *qtr++;
	strcpy(ptr, "_sskey.txt");
}
void convert_txt_to_bin(char *filename, char *arg1) {
	strcpy(filename, arg1);
	int len = strlen(filename);
	filename[len - 4] = '_';
	filename[len - 3] = 'c';
	filename[len - 2] = 'o';
	filename[len - 1] = 'm';
	filename[len] = 'p';
	filename[len + 1] = 'r';
	filename[len + 2] = 'e';
	filename[len + 3] = 's';
	filename[len + 4] = 's';
	filename[len + 5] = '.';
	filename[len + 6] = 'b';
	filename[len + 7] = 'i';
	filename[len + 8] = 'n';
	filename[len + 9] = '\0';
}
void convert_pass_to_bin(char *passfile, char *arg2) {
	strcpy(&arg2[strlen(arg2)], "_shortkey.bin");
	strcpy(passfile, arg2);
}
/******************************************************************************
* COMMON WORD SUBSTITUTION FUNCTIONS
******************************************************************************/
void splice_str(char *s, char *sub, int splice_len, int size) {
	char temp[size];
	sprintf(temp, "%s%s", sub, s + splice_len);
	strcpy(s, temp);
}
int delta_sub_words(char *s, int total_len, char remove[][50], char insert[][50]) {
	int count = 0, found, word_len, i, j;
	for(i = 0; i < 224; i++) {
		char *p = s;
		word_len = strlen(remove[i]);
		found = 0;
		while(*p != '\0') {
			if(*p == remove[i][0]) {
				for(j = 0; j < word_len; j++) if(*(p + j) != remove[i][j]) break;
				if(j == word_len) { /* if word in s is common word */
					splice_str(p, insert[i], word_len, total_len);
					if(zip_info == 1 && found == 0) { /* store common words for 'info' command */
						cw_idxs[count] = i;
						count++;
						found++;
					}
				}
			} else if(*p == '`' || *p == '!' || *p == '?') { /* avoid chaining words: themself => !uself => !?slf */
				p++;
			}
			p++;
		}
	}
	return count;
}
void print_cw_used(int sub_size) {
	printf("SUBSTITUTIONS:\n");
	for(int i = 0; i < sub_size; i++) {
		printf("=> %s: %s\n", cw_keys[cw_idxs[i]], cw_word[cw_idxs[i]]);
	}
}
/******************************************************************************
* CHAR HANDLER FUNCTIONS
******************************************************************************/
void rm_nxt_ch(char *q, int ss_ref_idx) {
	sprintf(q, "%c%s", ss_refs[ss_ref_idx], q + 1); /* replace 1st char with symbol */
	memmove(q + 1, q + 2, strlen(q + 1)); /* remove 2nd char */
}
void modify_s(char *s, int sp_flag) {
	char *p = s, space = ' ', under_s = '_';
	if(sp_flag == 0) { /* lowercase */
		while(*p != '\0') {
			*p = tolower(*p);
			p++;
		}
	} else if(sp_flag == 1) { /* lowercase + underscores for spaces + punc subs */
		while(*p != '\0') {
			*p = tolower(*p);
			if(*p == space) {
				*p = under_s;
			} else if(DASH_PUNC(*p) == 1) {
				*p = '-';
			} else if(*p == '"'){
				*p = '\'';
			} else if(*p == 'q' && *(p + 1) == 'u') {
				*p = 'k';
				*(p + 1) = 'w';
				p++;
			}
			p++;
		}
	} else { /* spaces for underscores (decrypt) && kw => qu */
		while(*p != '\0') {
			if(*p == under_s) {
				*p = space;
			} else if(*p == 'k' && *(p + 1) == 'w') {
				*p = 'q';
				*(p + 1) = 'u';
				p++;
			}
			p++;
		}
	}
}
/******************************************************************************
* DE/COMPRESSION FUNCTIONS
******************************************************************************/
void s_decompress(char ss[][152], char *s) {
	char *p = s, t[152];
	while(*p != '\0') {
		if(is_ss_ref(0, *p) == 1) {
			sprintf(t, "%s%s", ss[ss_ch_index(*p)], p+1);
			sprintf(p, "%s", t);
			p = s;
		} else {
			p++;
		}
	}
}
int s_compress(char ss[][152], char *s) { /* returns # of substrings */
	modify_s(s, 1); /* lowify & ' ' => '_' */
	int sub_word_count = delta_sub_words(s, 152, cw_word, cw_keys); /* remove common words, insert keys */
	if(zip_info == 1) {
		printf("SUBD WORDS => LEN: %lu, STR: %s\n", strlen(s), s);
		print_cw_used(sub_word_count); /* print common words subbed and associated keys */
	}
	int ss_idx = 0, found;
	char *p = s, *r;
	while(*(p + 2) != '\0') { /* first ss instance */
		r = p + 2;
		found = 0;
		while(*(r + 1) != '\0') { /* second ss instance */
			if(*r == *p && *(r + 1) == *(p + 1)) {
				found++;
				rm_nxt_ch(r, ss_idx);
			}
			r++;
		}
		if(found > 0) { /* if ss found, restart search for compounded ss */
			store_ss(ss, p, ss_idx);
			rm_nxt_ch(p, ss_idx);
			ss_idx++;
			p = s;
		} else {
			p++;
		}
	}
	return clean_ss(ss, s, ss_idx);
}
void process_split_s(char ss[][152], char *s, char *s_compress_storage, char *arg2) {
	if(zip_info == 1) printf("\nORIGINALLY => LEN: %lu, STR: %s\n", strlen(s), s);
	int ss_total = s_compress(ss, s);
	if(zip_info == 1) printf("COMPRESSED => LEN: %lu, STR: %s\n", strlen(s), s);
	write_pass_ss_keys(ss, ss_total, arg2); /* store ss keys in bin"password"file */
	strcpy(s_compress_storage, s); /* store compressed string */
	if(zip_info == 1) print_ss(ss, ss_total);
}
/******************************************************************************
* (S)UB(S)TRING REFERENCE KEY ANALYSIS FUNCTIONS
******************************************************************************/
int is_ss_ref(int start, char ch) {
	char *p = &ss_refs[start];
	while(*p != '\0') {
		if(*p == ch) return 1;
		p++;
	}
	return 0;
}
int ss_ch_index(char ch_c_idx) {
	if(CH_IS_GCAP(ch_c_idx) == 1) {
		return (int)(ch_c_idx - 65);
	} else if(ch_c_idx >= '<' && ch_c_idx <= '>') {
		return (int)(ch_c_idx - 32);
	} else if(ch_c_idx == '#' || ch_c_idx == '$') {
		return (int)(ch_c_idx - 9);
	} else if(ch_c_idx == '@') {
		return (int)(ch_c_idx - 33);
	} else if(ch_c_idx == '[') {
		return (int)(ch_c_idx - 59);
	} else if(ch_c_idx == ']' || ch_c_idx == '^') {
		return (int)(ch_c_idx - 55);
	} else {
		return (int)(ch_c_idx - 88);
	}
}
char adjust_ref_char(char ss_ref_ch) {
	if(ss_ref_ch == '#') { /* become Z */
		return (char)(ss_ref_ch + 55);
	} else if(ss_ref_ch == '<') { /* become $ */
		return (char)(ss_ref_ch - 24);
	} else if(ss_ref_ch == '@' || ss_ref_ch == ']') { /* become > OR [ */
		return (char)(ss_ref_ch - 2);
	} else if(ss_ref_ch == '[') { /* become @ */
		return (char)(ss_ref_ch - 27);
	} else if(ss_ref_ch == '{') { /* become ^ */
		return (char)(ss_ref_ch - 29);
	} else { /* become previous ss_refs elem */
		return (char)(ss_ref_ch - 1);
	}
}
/******************************************************************************
* (S)UB(S)TRING FUNCTIONS
******************************************************************************/
void store_ss(char ss[][152], char *p, int ss_idx) {
	sprintf(ss[ss_idx], "%c%c", *p, *(p + 1));
}
int clean_ss(char ss[][152], char *s, int ss_total) {
	char *p;
	int nest_idx = 0, nested_ss[40]; /* nested ss only ref'd by other ss, arr holds their idxs */
	for(int i = 0; i < ss_total; i++) {
		p = s;
		while(*p != '\0') {
			if(*p == ss_refs[i]) break; /* if s char == ss ref */
			p++;
		}
		if(*p == '\0') { /* if ss_refs[i] !in s, i == nested ss ref idx */
			nested_ss[nest_idx] = i; /* nested ss idxs stored sequentially relative to ss_refs order */
			nest_idx++;
		}
	}
	splice_ss(ss, nested_ss, nest_idx, ss_total); /* put nested ss vals in ss referencing them */
	trim_ss(ss, s, nested_ss, nest_idx, ss_total); /* adjust ss & s references as per rmvd nested ss */
	return (ss_total - nest_idx); /* actual ss total = original ss total - nested ss total */
}
void splice_ss(char ss[][152], int nested_ss[40], int nest_total, int ss_total) {
	for(int i = 0; i < nest_total; i++) { /* traverse nested ss array */
		for(int j = 0; j < ss_total; j++) { /* traverse ss array */
			int curr_ss_len = strlen(ss[j]);
			for(int l = 0; l < curr_ss_len; l++) { /* traverse ss */
				if(ss[j][l] == ss_refs[nested_ss[i]]) { /* if ss has nested ss reference */
					char nested_elem[152]; /* clone to hold nested ref value */
					strcpy(nested_elem, ss[nested_ss[i]]); /* clone nested ref val */
					/* splice post-nested ref ss tail to end of nested ref val */
					strcpy(&nested_elem[strlen(nested_elem)], &ss[j][l + 1]); 
					strcpy(&ss[j][l], nested_elem); /* splice nested ref val + ss end to ss ref position */
				}
			}
		}
	}
}
void trim_ss(char ss[][152], char *s, int nested_ss[40], int nest_total, int ss_total) {
	int i, j;
	for(i = 0; i < nest_total; i++) { /* delete nested-ss */
		char *p = s;
		for(j = nested_ss[i]; j < (ss_total - 1); j++) {
			strcpy(ss[j], ss[j + 1]); /* shift up non-nested ss to delete nested-ss */
			int curr_ss_len = strlen(ss[j]);
			for(int l = 0; l < curr_ss_len; l++) {
				if(is_ss_ref((nested_ss[i]+ 1), ss[j][l]) == 1)/* ss-ss references -= 1 ss_refs val as per del */
					ss[j][l] = adjust_ref_char(ss[j][l]); /* IF^START 1 AFTER BECAUSE SS CAN ONLY BE REF'D BY OTHER SS BELOW THEM */
			}
		}
		for (int k = i; k < nest_total; k++) {
			nested_ss[k] = adjust_ref_char(nested_ss[k]); /* nested-ss idxs -= 1 ss_refs val as per del */
		}
		ss[j][0] = '\0';
		while(*p != '\0') { /* s ss references -= 1 ss_refs val as per del */
			if(is_ss_ref((nested_ss[i]+ 1), *p) == 1) /* if *p = nested ref */
				*p = adjust_ref_char(*p);
			p++;
		}
	}
}
void print_ss(char ss[][152], int ss_total) {
	printf("COMPR SUBSTRS:\n");
	for(int i = 0; i < ss_total; i++) printf("=> %c: %s\n", ss_refs[i], ss[i]);
}
/******************************************************************************
* HIDE INT
******************************************************************************/
void hide_int(char *arg2, char *arg1, char *s) {
	int newl_total = count_newl(s), end_index, i, j, s_size = strlen(s);
	int data_chunks = (newl_total + 1 + ((s_size - 1)/5));
	unsigned int num_store[data_chunks]; /* holds n's */
	char filename[75], passfile[75], *q = s; /* current place in str */
	convert_pass_to_bin(passfile, arg2); /* convert password to bin file */

	for(i = 0; *q != '\0'; i++) { /* convert letters to ints */
		char *p = q, s_temp[s_size];
		strcpy(s_temp, p);
		end_index = find_newl(s_temp); 
		s_temp[end_index] = '\0'; /* dedicate storage int 5 letters or up to \n */
		q += end_index; /* traverse string to new 5 letter chunk or up to \n */
		num_store[i] = str_to_int(s_temp); /* store int */
	}
	unsigned int write_arr[i];
	for(int m = 0; m < i; m++) write_arr[m] = num_store[m]; /* don't write excess numbers */
	/* STORE INTS AS SHORTS AND REMOVE DUPLICATES */
	unsigned short short_store[i * 2];
	for(j = 0; j < (i * 2); j += 2) { /* convert int from array into 2 shorts in array */
		unsigned short temp_1, temp_2;
		int_to_shorts(write_arr[j / 2], &temp_1, &temp_2); 
		short_store[j] = temp_1;
		short_store[j + 1] = temp_2;
	}
	/* COUNT DOUBLE INSTANCES OF SHORT AND MAKE ARRAY OF THEM */
	unsigned short short_copies[j]; /* sub short duplicates with char references */
	int total_short_dubs = count_double_shorts(short_copies, short_store, j);
	int short_order[j]; /* get order of chars(0) and shorts(1) to store for reading */
	int order_length = get_short_char_order(short_order, short_copies, short_store, j, total_short_dubs);
	int const BUFF_BIT_SIZE = order_length;
	int NUMBER_OF_BIT_PACKETS = 1 + ((BUFF_BIT_SIZE - 1) / 8);
	/************************************ PACK ************************************/
	char packed[NUMBER_OF_BIT_PACKETS]; /* INT ORDER IS BIT-PACKED INTO THIS CHAR ARRAY */
	packer(BUFF_BIT_SIZE, NUMBER_OF_BIT_PACKETS, short_order, packed);
	FILE *fp;
	if((fp = fopen(arg2, "wb")) == NULL) {
		printf("\n\n(!!!) ERROR WRITING SHORT COMPR INFORMATION(!!!)\n");
		return;
	}
	fwrite(&order_length, sizeof(int), 1, fp); /* length of string */
	fwrite(packed, sizeof(char), NUMBER_OF_BIT_PACKETS, fp); /* packed short/char order of 1/0's into char array */
	fwrite(&total_short_dubs, sizeof(int), 1, fp); /* number of dubplicate shorts (duplicate key length) */
	fwrite(short_copies, sizeof(unsigned short), total_short_dubs, fp); /* duplicate short array */
	fclose(fp);
	
	convert_txt_to_bin(filename, arg1); /* write compressed file to binary file with name of original .txt */
	FILE *ft;
	if((ft = fopen(filename, "wb")) == NULL) {
		printf("\n\n(!!!) ERROR WRITING COMPRESSED SHORTS(!!!)\n");
		return;
	}
	int byte_count = 0;
	for(int k = 0; k < order_length; k++) { /* store shorts and chars (shorts and duplicate keys) */
		int short_copy_idx = short_num_copies(short_store[k], short_copies, total_short_dubs);
		if(short_copy_idx == -1) {
			fwrite(&short_store[k], sizeof(unsigned short), 1, ft);
			byte_count += 2;
		} else {
			char ch = (char)short_copy_idx;
			fwrite(&ch, sizeof(char), 1, ft);
			byte_count++;
		}
	}
	fclose(ft);

	float bytes_saved = 100*(1.000000 - ((float)byte_count/(float)original_bytes)); /* print compr stats */
	printf("\n==============================================================================");
	printf("\n%s ==COMPRESS=> %s\n", arg1, filename);
	printf(">> BYTES => UNCOMPRESSED: %d, COMPRESSED: %d, COMPRESSION RATE: %.2f%%\n", original_bytes, byte_count, bytes_saved);
	printf("==============================================================================\n\n");
}
/******************************************************************************
* SHOW INT
******************************************************************************/
void show_int(char *arg2, char *arg1) {
	int i, j, k, last_newl = 0, current_idx = 0, end_idx, copy_count = 0;
	char newl_buffer[3000], s_store[30000], store_temp[5], filename[75], passfile[75]; /* newl_buffer = \n split_str, s_store holds char from ints */
	convert_txt_to_bin(filename, arg1); /* original .txt filename to binary file */
	convert_pass_to_bin(passfile, arg2); /* convert password to bin file */
	/*********************************** UNPACK ***********************************/
	int order_array_len, total_short_dubs;
	FILE *fp;
	if((fp = fopen(arg2, "rb")) == NULL) {
		printf("\n\n(!!!) ERROR READING SHORT COMPR INFORMATION (!!!)\n");
		return;
	}
	fread(&order_array_len, sizeof(int), 1, fp); /* get unpacked order array length */

	int const BUFF_BIT_SIZE = order_array_len; /* scraped from beginning of file */
	int NUMBER_OF_BIT_PACKETS = 1 + ((BUFF_BIT_SIZE - 1) / 8);
	char packed[NUMBER_OF_BIT_PACKETS]; /* STORE THE PACKED INTS AS CHARS INTO THIS */
	int unpacked[BUFF_BIT_SIZE]; /* UNPACK INTO THIS */
	fread(packed, sizeof(char), NUMBER_OF_BIT_PACKETS, fp); /* read packed order */
	unpacker(BUFF_BIT_SIZE, NUMBER_OF_BIT_PACKETS, unpacked, packed); /* unpack order into unpacked array */

	fread(&total_short_dubs, sizeof(int), 1, fp);
	unsigned short short_copies[total_short_dubs];
	fread(short_copies, sizeof(unsigned short), total_short_dubs, fp);
	fclose(fp);

	FILE *ft;
	if((ft = fopen(filename, "rb")) == NULL) {
		printf("\n\n(!!!) ERROR READING COMPRESSED SHORTS (!!!)\n");
		return;
	}
	unsigned short short_store[order_array_len];
	for(int k = 0; k < order_array_len; k++) {
		char ch_temp;
		unsigned short sh_temp;
		int is_short = unpacked[k]; /* for readability */
		if(is_short == 1) {
			fread(&sh_temp, sizeof(unsigned short), 1, ft);
			short_store[k] = sh_temp;
		} else if(is_short == 0) {
			fread(&ch_temp, sizeof(char), 1, ft);
			int ch_idx = (int)ch_temp;
			if(ch_idx < 0) ch_idx += 256; /* casting int -> char -> int, 0-128: #, 128-256: # - 256 */
			short_store[k] = short_copies[ch_idx];
		}
	}
	fclose(ft);
	/* convert 2 shorts into 1 int to be decompressed into chars */
	int int_array_length = (order_array_len / 2);
	unsigned int read_arr[int_array_length]; /* trimmed short_store array to not write excess numbers */
	for(int m = 0; m < order_array_len; m += 2) read_arr[m / 2] = shorts_to_int(short_store[m], short_store[m + 1]);

	for(j = 0; j < int_array_length; j++) { /* convert ints to letters */
		unsigned int n_temp = read_arr[j]; /* int to decompress to str */
		for(k = 0; k < 7 && n_temp != 0; k++) n_temp >>= 6; /* get number of letters in int */
		end_idx = int_to_str(read_arr[j], store_temp, k); /* return number of letters (5 or till \n) */
		strcpy(&s_store[current_idx], store_temp); /* add str to rest of decompressed str's */
		current_idx += end_idx; /* increment s_store index by number of letters in str just stored */
		if(end_idx == k - 1) { /* newline detected */
			two_ptr_str_splice(newl_buffer, &s_store[last_newl], &s_store[current_idx]); /* get string since last \n */
			process_split_int(newl_buffer); /* print the split_str (between \n's) */
			last_newl = current_idx; /* beginning of next split_str chunk (last \n position) is current position */
		}
	}
	for(i = 0; i < chunk_count; i++) {
		char *p = s_max_buffer;
		if(i != 0) while(*p != '\0') p++;
		sprintf(p, "%s", s_compress_storage[i]);
	}
	remove(filename); /* delete compression container */
	remove(arg2); /* delete compression information container (short keys) */
}
void process_split_int(char *s_buffer) {
	char *p = s_buffer;
	if(zip_info == 1) printf("\nCOMPRESSED: %s\n", s_buffer);
	s_decompress(ss_array_matrix[chunk_count], s_buffer);
	delta_sub_words(s_buffer, 152, cw_keys, cw_word); /* remove keys, insert words */
	modify_s(s_buffer, 2); /* '_' => ' ' */
	if(zip_info == 1) printf("DECOMPRESS: %s\n", s_buffer);
	strcpy(s_compress_storage[chunk_count], s_buffer);
	chunk_count++;
}
/******************************************************************************
* INT COMPRESSION / DECOMPRESSION
******************************************************************************/
int str_to_int(char *s) { /* STORE LETTERS => COMPRESS */
	int s_size = strlen(s), ch_shift, i;
	char *p = s;
	unsigned int n = 0;
	for(i = 0; i < s_size; i++, p++) { /* processes front-to-back */
		ch_shift = get_ch_shift(*p);
		n |= (unsigned int)((*p) - ch_shift);
		n <<= 6 * (i < s_size - 1);
	}
	return n;
}
int int_to_str(unsigned int n, char *t, int s_size) { /* READ LETTERS => DECOMPRESS */
	int num_shift, j;
	char temp_t[6];
	for(j = s_size - 1; j >= 0; j--) { /* processes back-to-front */
		num_shift = get_num_shift(n & 63);
		temp_t[j] = (char)((n & 63) + num_shift);
		n >>= 6 * (j > 0);
	}
	temp_t[s_size] = '\0';
	strcpy(t, temp_t);
	if(temp_t[s_size - 1] == '\n') { /* don't copy newlines */
		t[s_size - 1] = '\0';
		return s_size - 1;
	}
	return s_size;
}
/******************************************************************************
* NEWLINE/SPLICE FUNCTIONS
******************************************************************************/
int count_newl(char *s) {
	int count = 0;
	char *p = s;
	while(*p != '\0') {
		if(*p == '\n') count++;
		p++;
	}
	return count;
}
int find_newl(char *s) {
	int i;
	for(i = 0; i < 5; i++) if(s[i] == '\n') return i + 1; /* return position after newline */
	return i; /* return end position of 5 letter chunk */
}
void two_ptr_str_splice(char *t, char *first, char *last) {
	char *p = first;
	int i = 0;
	strcpy(t, first);
	while(p != last) {
		i++;
		p++;
	}
	t[i] = '\0';
}
/******************************************************************************
* CONVERT 2 UNSIGNED SHORTS TO 1 INT AND VISE-VERSA
******************************************************************************/
unsigned int shorts_to_int(unsigned short n1, unsigned short n2) {
	unsigned int num1 = ((unsigned int)(n1) << 16), num2 = ((unsigned int)n2), num;
	num = (num2 | num1);
	return num;
}
void int_to_shorts(unsigned int num, unsigned short *n1, unsigned short *n2) {
	unsigned short num1 = ((unsigned short)(num >> 16)), num2 = ((unsigned short)(num & 65535));
	*n1 = num1;
	*n2 = num2; /* (2^16) - 1 */
}
/******************************************************************************
* COUNT DUPLICATE INTS/SHORTS IN FINAL INT ARRAY
******************************************************************************/
int get_short_char_order(int short_order[], unsigned short short_copies[], unsigned short num_store[], int size, int cpy_size) {
	int i, j = 0, count = 0;
	for(i = 0; i < size; i++) { /* size represents how many chars can fit bitpacked (1/0)'s *//* if unsigned short */
		(short_num_copies(num_store[i], short_copies, cpy_size) == -1) ? (short_order[i] = 1) : (short_order[i] = 0);
	}
	return i;
}
int short_num_copies(unsigned short num_short, unsigned short short_copies[], int size) {
	int i;
	for(i = 0; i < size; i++) if(short_copies[i] == num_short) return i; /* is copy and will be char */
	return -1; /* not copy and will be unsigned short */
}
int count_double_shorts(unsigned short short_copies[], unsigned short num_store[], int size) {
	int found, total = 0, count = 0, i, j;
	for(i = 0; i < size - 1; i++) {
		found = 0;
		for(j = i + 1; j < size; j++) {
			if(num_store[i] == num_store[j]) {
				count++; /* total instances of a duplicate short */
				if(short_num_copies(num_store[j], short_copies, size) == -1 && found == 0) {
					short_copies[total] = num_store[j];
					total++; /* total number of individual shorts being duplicated */
					found++;
				}
			}
		}
	}
	return total;
}
/******************************************************************************
* ADJUST CHAR FOR INT COMPR REPRESENTATION ('A' => 1, '\n' => 63)
******************************************************************************/
int get_ch_shift(char ch) {
	if(ch >= 'a' && ch <= 'z' == 1) { /* 'a' => 1, 'z' => 26 */
		return 96;
	} else if(ch >= 'A' && ch <= 'Z' == 1) { /* 'A' => 27, 'Z' => 52 */
		return 38;
	} else if(ch == '.') { /* 46 => 53 */
		return -7;
	} else if(ch == '_') { /* 95 => 54 */
		return 41;
	} else if(ch == ',') { /* 44  => 55 */
		return -11;
	} else if(ch == '\'') { /* 39  => 56 */
		return -17;
	} else if(ch == '?') { /* 63  => 57 */
		return 6;
	} else if(ch == '!') { /* 33  => 58 */
		return -25;
	} else if(ch == '`') { /* 96  => 59 */
		return 37;
	} else if(ch == '-') { /* 45 => 60 */
		return -15;
	} else if(ch == '#' || ch == '$') { /* 35 => 61 || 36 => 62 */
		return -26;
	} else if(ch == '\n') { /* 10 => 63 */
		return -53;
	}
	return 0;
}
int get_num_shift(int num) {
	if(num >= 1 && num <= 26) { /* 'a' => 1, 'z' => 26 */
		return 96;
	} else if(num >= 27 && num <= 52) { /* 'A' => 27, 'Z' => 52 */
		return 38;
	} else if(num == 53) { /* 46 => 53 */
		return -7;
	} else if(num == 54) { /* 95 => 54 */
		return 41;
	} else if(num == 55) { /* 44  => 55 */
		return -11;
	} else if(num == 56) { /* 39  => 56 */
		return -17;
	} else if(num == 57) { /* 63  => 57 */
		return 6;
	} else if(num == 58) { /* 33  => 58 */
		return -25;
	} else if(num == 59) { /* 96  => 59 */
		return 37;
	} else if(num == 60) { /* 45 => 60 */
		return -15;
	} else if(num == 61 || num == 62) { /* 35 => 61 || 36 => 62 */
		return -26;
	} else if(num == 63) { /* 10 => 63 */
		return -53;
	}
	return 0;
}
/******************************************************************************
* WRITE (BIT-PACK) INT ARR IN CHAR THEN READ (UNPACK) BACK INTO INT ARR
******************************************************************************/
void fill_temp_with_buffer(int temp[], int temp_size, int buffer[], int i) {
	for(int j = (8 * i); j < ((8 * i) + temp_size); j++) temp[j - (8 * i)] = buffer[j];
}
int get_shift_number(int BIT_SIZE) {
	int shift = 2;
	for(int i = 1; i < BIT_SIZE - 1; shift *= 2, i++);
	return shift;
}
void pack(int x[], int BIT_SIZE, char packed[], int pack_idx) {
	int shift = get_shift_number(BIT_SIZE); /* FUNCITON TO MAKE SHIFT */
	char c = 0;
	for(int i = 0; i < BIT_SIZE; i++) { /* stores array from right to left => PRINT STORE ARRAY BACKWARDS IN FILE 2B READ FORWARDS */
		c |= x[i];
		if(i < BIT_SIZE - 1) c <<= 1;
	}
	packed[pack_idx] = c;
}
void unpack(int *int_array, int BIT_SIZE, char pack_ch) { /* pass each char in packed */
	int y[BIT_SIZE], shift = get_shift_number(BIT_SIZE); /* FUNCITON TO MAKE SHIFT */
	for(int j = BIT_SIZE - 1; j >= 0; j--) { /* reads bits in char from left to write into int array */
		y[j] = (((pack_ch << j) & shift) / shift);
	}
	for(int k = 0; k < BIT_SIZE; k++) {
		*int_array = y[k];
		if(k < BIT_SIZE - 1) int_array++;
	}
}
void packer(int BUFF_BIT_SIZE, int NUMBER_OF_BIT_PACKETS, int buffer[], char packed[]) {
	int temp_size;
	for(int i = 0; i < NUMBER_OF_BIT_PACKETS; i++) {
		if(LESS_THAN_8_BITS_LEFT(BUFF_BIT_SIZE, i) == 1) { /* if < 8 bits left */
			temp_size = REMAINING_BITS(BUFF_BIT_SIZE, i); /* get remaining bits */
		} else {
			temp_size = 8;
		}
		int temp[temp_size];
		fill_temp_with_buffer(temp, temp_size, buffer, i);
		pack(temp, temp_size, packed, i);
	}
}
void unpacker(int BUFF_BIT_SIZE, int NUMBER_OF_BIT_PACKETS, int buffer[], char packed[]) {
	int temp_size;
	for(int i = 0; i < NUMBER_OF_BIT_PACKETS; i++) {
		if(LESS_THAN_8_BITS_LEFT(BUFF_BIT_SIZE, i) == 1) { /* if < 8 bits left */
			temp_size = REMAINING_BITS(BUFF_BIT_SIZE, i); /* get remaining bits */
		} else {
			temp_size = 8;
		}
		unpack(&buffer[(i * 8)], temp_size, packed[i]);
	}
}
