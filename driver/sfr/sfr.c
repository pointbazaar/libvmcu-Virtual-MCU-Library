/* Driver to analyze SFRs  */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

// libvmcu
#include "libvmcu_analyzer.h"
#include "libvmcu_system.h"

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./sfr <hexfile>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t *report = vmcu_analyze_ihex(argv[1], m328p);

    if(report == NULL) {

        vmcu_model_dtor(m328p);
        return EXIT_FAILURE;
    }

    for(int32_t i = 0; i < report->n_sfr; i++) {

        vmcu_sfr_t *sfr = &report->sfr[i];
        printf("SFR ID: %d\n", sfr->id);

        for(int32_t j = 0; j < sfr->n_xref; j++) {

            vmcu_xref_t *x = &sfr->xref[j];

            printf("  xref from 0x%04x ", x->i->addr);
            printf("(%s)\n", x->i->mnem);
        }

        printf("\n");
    }

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}
