#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include <stdatomic.h>
#include "src/galton/random/random_utils.h"

atomic_int random_seed = 0;

void init_random() {
    srand(to_us_since_boot(get_absolute_time()));
    atomic_store(&random_seed, rand());  // Define o seed inicial
}

// Função que gera um número com distribuição normal padrão (média 0, desvio 1)
double generate_normal() {
    double u1 = ((rand() + 1.0) / (RAND_MAX + 2.0));  // evitar log(0)
    double u2 = ((rand() + 1.0) / (RAND_MAX + 2.0));
    return sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
}

// Gera 0 ou 1 com base em distribuição normal
int random_direction() {
    int seed = atomic_load(&random_seed);
    srand(seed);  // Atualiza seed localmente
    double normal_value = generate_normal();  // valor normalmente distribuído com média 0

    atomic_store(&random_seed, rand());  // Atualiza seed global de forma atômica

    // Limiar em 0: aprox. 50% para cada lado, mas com curva Gaussiana
    return (normal_value >= 0) ? 1 : 0;
}
