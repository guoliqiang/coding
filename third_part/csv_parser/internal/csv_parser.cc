#include "third_part/csv_parser/public/csv_parser.h"
#include "base/public/logging.h"

bool csv_parser::init(FILE * input_file_pointer) {
	input_fp = input_file_pointer;
	if (input_fp == NULL) {
		LOG(FATAL) << "unable to open input file from file pointer";
		return false;
	}
	// Resetting the internal pointer to the beginning of the stream
	rewind(input_fp);
	more_rows = true;
	_skip_lines();
	return true;
}

bool csv_parser::init(const char * input_file) {
	const size_t filename_length = strlen(input_file);
	if (!filename_length) {
		LOG(FATAL) << "invalid input file " << input_file;
		return false;
	}
	input_filename = (char *) malloc(filename_length + 1);
	if (input_filename == NULL) {
		LOG(FATAL) << "unable to allocate memory for file name buffer:"
               << input_file;
		return false;
	}
	memset(input_filename, 0, filename_length + 1);
	strcpy(input_filename, input_file);
	input_fp = fopen(input_file, "r");
	if (input_fp == NULL) {
		LOG(FATAL) << "unable to open input file:" << input_file;
		CSV_PARSER_FREE_BUFFER_PTR(input_filename);
		return false;
	}
	more_rows = true;
	_skip_lines();
	return true;
}

void csv_parser::set_enclosed_char(char fields_enclosed_by,
    enclosure_type_t enclosure_mode) {
	if (fields_enclosed_by != 0) {
		enclosed_char   = fields_enclosed_by;
		enclosed_length = 1U;
		enclosure_type  = enclosure_mode;
	}
}

void csv_parser::set_field_term_char(char fields_terminated_by) {
	if (fields_terminated_by != 0) {
		field_term_char   = fields_terminated_by;
		field_term_length = 1U;
	}
}

void csv_parser::set_line_term_char(char lines_terminated_by) {
	if (lines_terminated_by != 0) {
		line_term_char   = lines_terminated_by;
		line_term_length = 1U;
	}
}

csv_row csv_parser::get_row(void) {
	csv_row current_row;
	unsigned int line_length = 0U;
	char * line = NULL;
	_read_single_line(&line, &line_length);
	switch(enclosure_type) {
		case ENCLOSURE_NONE : {
			_get_fields_without_enclosure(&current_row, line, &line_length);
		  break;
    }
		case ENCLOSURE_REQUIRED : {
			_get_fields_with_enclosure(&current_row, line, &line_length);
		  break;
    }
		case ENCLOSURE_OPTIONAL : {
			_get_fields_with_optional_enclosure(&current_row, line, &line_length);
		  break;
    }
		default :
			_get_fields_with_optional_enclosure(&current_row, line, &line_length);
		  break;
	}
	CSV_PARSER_FREE_BUFFER_PTR(line);
	record_count++;
	return current_row;
}

void csv_parser::_skip_lines(void) {
	unsigned int number_of_lines_to_ignore = abs((int) ignore_num_lines);
	while (has_more_rows() && number_of_lines_to_ignore) {
		const csv_row row = get_row();
		number_of_lines_to_ignore--;
	}
	record_count = 0U;
}

void csv_parser::_get_fields_without_enclosure(csv_row_ptr row,
    const char * line, const unsigned int * line_length) {
	char * field = NULL;
	if (*line_length > 0) {
		field = (char *) malloc(*line_length);
		memset(field, 0, *line_length);
		register unsigned int field_start = 0;
		register unsigned int field_end = 0;
		register unsigned int char_pos = 0;
		while (char_pos < *line_length) {
			char curr_char = line[char_pos];
			if (curr_char == field_term_char) {
				field_end = char_pos;
				const char * field_starts_at = line + field_start;
				const unsigned int field_width = field_end - field_start;
				memcpy(field, field_starts_at, field_width);
				field[field_width] = 0x00;
				string field_string_obj = field;
				row->push_back(field_string_obj);
				field_start = char_pos + 1;
			} else if (curr_char == line_term_char) {
				field_end = char_pos;
				const char * field_starts_at = line + field_start;
				const unsigned int field_width = field_end - field_start;
				memcpy(field, field_starts_at, field_width);
				field[field_width] = 0x00;
				string field_string_obj = field;
				row->push_back(field_string_obj);
			}
			char_pos++;
		}
		CSV_PARSER_FREE_BUFFER_PTR(field);
	}
}

void csv_parser::_get_fields_with_enclosure(csv_row_ptr row, const char * line,
    const unsigned int * line_length) {
	char * field = NULL;
	if (*line_length > 0) {
		field = (char *) malloc(*line_length);
		memset(field, 0, *line_length);
		register unsigned int current_state = 0;
		register unsigned int field_start = 0;
		register unsigned int field_end = 0;
		register unsigned int char_pos = 0;
		while (char_pos < *line_length) {
			char curr_char = line[char_pos];
			if (curr_char == enclosed_char) {
				current_state++;
				// Lets find out if the enclosure character encountered is
				// a 'real' enclosure character or if it is an embedded character that
				// has been escaped within the field.
				register char previous_char = 0x00;
				if (char_pos > 0U) {
					// The escaped char will have to be the 2rd or later character.
					previous_char = line[char_pos - 1];
					if (previous_char == escaped_char) {
						--current_state;
					}
				}
				if (current_state == 1U && previous_char != escaped_char) {
					field_start = char_pos;
				} else if (current_state == 2U) {
					field_end = char_pos;
					const char * field_starts_at = line + field_start + 1U;
					const unsigned int field_width = field_end - field_start - 1U;
					memcpy(field, field_starts_at, field_width);
					field[field_width] = 0x00;
					string field_string_obj = field;
					row->push_back(field_string_obj);
					current_state = 0U;
				}
			}
			char_pos++;
		}
		// If no enclosures were found in this line, the entire line becomes
    // the only field.
		if (0 == row->size()) {
			string entire_line = line;
			row->push_back(entire_line);
		} else if (current_state == 1U) {
			// The beginning enclosure character was found but
			// we could not locate the closing enclosure in the current line
			// So we need to copy the remainder of the line into the last field.
			// We do not need the starting enclosure character
			const char * field_starts_at = line + field_start + 1U;
			const unsigned int field_width = *line_length - field_start - 1U;
			memcpy(field, field_starts_at, field_width);
			field[field_width] = 0x00;
			string field_string_obj = field;
			row->push_back(field_string_obj);
		}
		CSV_PARSER_FREE_BUFFER_PTR(field);
	}
}

void csv_parser::_get_fields_with_optional_enclosure(csv_row_ptr row,
    const char * line, const unsigned int * line_length) {
	char * field = NULL;
	// How to extract the fields, when the enclosure char is optional.
	// This is very similar to parsing the document without enclosure but
  // with the following conditions. If the beginning char is an enclosure
  // character, adjust the starting position of the string by + 1.If the ending
  // char is an enclosure character, adjust the ending position by -1
	if (*line_length > 0) {
		field = (char *) malloc(*line_length);
		memset(field, 0, *line_length);
		register unsigned int field_start = 0;
		register unsigned int field_end = 0;
		register unsigned int char_pos = 0;
		while (char_pos < *line_length) {
			char curr_char = line[char_pos];
			if (curr_char == field_term_char) {
				field_end = char_pos;
				const char * field_starts_at = line + field_start;
				unsigned int field_width = field_end - field_start;
				const char line_first_char = field_starts_at[0];
				const char line_final_char = field_starts_at[field_width - 1];
				unsigned int first_adjustment =
            (line_first_char == enclosed_char) ? 1U : 0U;
				unsigned int final_adjustment =
            (line_final_char == enclosed_char) ? 2U : 0U;
				field_width = (field_width > 2U) ?
            (field_width - final_adjustment) : field_width;
				memcpy(field, field_starts_at + first_adjustment, field_width);
				field[field_width] = 0x00;
				string field_string_obj = field;
				row->push_back(field_string_obj);
				field_start = char_pos + 1;
			} else if (curr_char == line_term_char) {
				field_end = char_pos;
				const char * field_starts_at = line + field_start;
				unsigned int field_width = field_end - field_start;
				const char line_first_char = field_starts_at[0];
				const char line_final_char = field_starts_at[field_width - 1];
				unsigned int first_adjustment =
            (line_first_char == enclosed_char) ? 1U : 0U;
				unsigned int final_adjustment =
            (line_final_char == enclosed_char) ? 2U : 0U;
				field_width = (field_width > 2U) ?
            (field_width - final_adjustment) : field_width;
				memcpy(field, field_starts_at + first_adjustment, field_width);
				field[field_width] = 0x00;
				string field_string_obj = field;
				row->push_back(field_string_obj);
			}
			char_pos++;
		}
		CSV_PARSER_FREE_BUFFER_PTR(field);
	}
}

void csv_parser::_read_single_line(char ** buffer, unsigned int * buffer_len) {
	long int original_pos = ftell(input_fp);
	long int current_pos  = original_pos;
	register int current_char = 0;
	while (true) {
		current_char = fgetc(input_fp);
		if (current_char == EOF) {
			more_rows = false;
			break;
		} else if (current_char == line_term_char) {
			current_pos++;
			break;
		} else {
			current_pos++;
		}
	}
	if (more_rows) {
		current_char = fgetc(input_fp);
		more_rows = (current_char == EOF) ? false : true;
	}
	const size_t length_of_row = current_pos - original_pos;
	if (length_of_row > 0) {
		*buffer_len = length_of_row * sizeof(char) + 1;
		*buffer = (char *) realloc(*buffer, *buffer_len);
		memset(*buffer, 0, *buffer_len);
		fseek(input_fp, original_pos, SEEK_SET);
		fread(*buffer, 1, length_of_row, input_fp);
	}
}
