#include "pbrain.h"

void pbrain_populate_procedure(bfx_t* bf) {
    size_t         i;
    pbrain_data_t* data                               = (pbrain_data_t*) bf->lang_data;

    data->procedures[data->procedures_len].start_idx  = bf->ip + 1;
    data->procedures[data->procedures_len].identifier = bf->tape[bf->tp];
    for (i = bf->ip; i < bf->program_len; i++) {
        if (bf->program[i] == ')') {
            data->procedures[data->procedures_len].end_idx = i;
            bf->ip                                         = i + 1;
            data->procedures_len++;
            return;
        }
    }
}
