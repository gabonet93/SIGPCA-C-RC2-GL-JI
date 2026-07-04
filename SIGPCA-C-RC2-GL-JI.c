#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ZONAS 5
#define DIAS_HISTORIAL 3

typedef struct {
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminantes;

typedef struct {
    float temperatura;
    float velocidad_viento;
    float humedad;
} Clima;

typedef struct {
    char nombre[50];
    Contaminantes actual;
    Contaminantes historial[DIAS_HISTORIAL];
    Clima clima_actual;
    Contaminantes prediccion;
    Contaminantes promedio_30dias; 
    
} ZonaUrbana;

const float LIMITE_PM25 = 15.0;
const float LIMITE_CO2  = 400.0;
const float LIMITE_SO2  = 40.0;
const float LIMITE_NO2  = 25.0;

// Prototipos de funciones
void ingresar_datos_manual(ZonaUrbana *zonas);
void calcular_promedios_historicos(ZonaUrbana *zonas);
void predecir_niveles_futuros(ZonaUrbana *zonas);
void evaluar_alertas_y_recomendaciones(ZonaUrbana *zonas);
void exportar_reporte(ZonaUrbana *zonas);
void mostrar_menu(ZonaUrbana *zonas);

int main() {
    ZonaUrbana zonas[NUM_ZONAS];
    
    strcpy(zonas[0].nombre, "Zona Norte (Industrial)");
    strcpy(zonas[1].nombre, "Zona Sur (Residencial)");
    strcpy(zonas[2].nombre, "Centro Historico");
    strcpy(zonas[3].nombre, "Zona Este (Comercial)");
    strcpy(zonas[4].nombre, "Zona Oeste (Periferia)");
    
    printf(" Sistema integral de gestion y prediccion de contaminacion del aire\n");
    
    mostrar_menu(zonas);
    
    return 0;
}

void ingresar_datos_manual(ZonaUrbana *zonas) {
    printf("           Ingreso de variables atmosfericas            \n");

    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("\n Registro de datos: %s \n", zonas[i].nombre);
        
        // Captura del historial por teclado
        for (int d = 0; d < DIAS_HISTORIAL; d++) {
            printf("  Historial - Dia %d - PM2.5 (ug/m3): ", d + 1);
            scanf("%f", &zonas[i].historial[d].pm25);
            printf("  Historial - Dia %d - CO2 (ppm): ", d + 1);
            scanf("%f", &zonas[i].historial[d].co2);
            printf("  Historial - Dia %d - SO2 (ug/m3): ", d + 1);
            scanf("%f", &zonas[i].historial[d].so2);
            printf("  Historial - Dia %d - NO2 (ug/m3): ", d + 1);
            scanf("%f", &zonas[i].historial[d].no2);
        }

        printf("  Lectura actual - PM2.5 (ug/m3): ");
        scanf("%f", &zonas[i].actual.pm25);
        printf("  Lectura actual - CO2 (ppm): ");
        scanf("%f", &zonas[i].actual.co2);
        printf("  Lectura actual - SO2 (ug/m3): ");
        scanf("%f", &zonas[i].actual.so2);
        printf("  Lectura actual - NO2 (ug/m3): ");
        scanf("%f", &zonas[i].actual.no2);

        // Captura de variables climatológicas de distribución
        printf("  Clima actual - Temperatura (C): ");
        scanf("%f", &zonas[i].clima_actual.temperatura);
        printf("  Clima actual - Vel. Viento (km/h): ");
        scanf("%f", &zonas[i].clima_actual.velocidad_viento);
        printf("  Clima actual - Humedad (%%): ");
        scanf("%f", &zonas[i].clima_actual.humedad);
    }
    printf("\n Todos los datos ambientales se cargaron en la memoria.\n");
}

void calcular_promedios_historicos(ZonaUrbana *zonas) {
    printf("\n  Promedios historicos del historial ingresado calculados \n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        float suma_pm25 = 0, suma_co2 = 0, suma_so2 = 0, suma_no2 = 0;
        
        for (int d = 0; d < DIAS_HISTORIAL; d++) {
            suma_pm25 += zonas[i].historial[d].pm25;
            suma_co2  += zonas[i].historial[d].co2;
            suma_so2  += zonas[i].historial[d].so2;
            suma_no2  += zonas[i].historial[d].no2;
        }
        
        zonas[i].promedio_30dias.pm25 = suma_pm25 / DIAS_HISTORIAL;
        zonas[i].promedio_30dias.co2  = suma_co2 / DIAS_HISTORIAL;
        zonas[i].promedio_30dias.so2  = suma_so2 / DIAS_HISTORIAL;
        zonas[i].promedio_30dias.no2  = suma_no2 / DIAS_HISTORIAL;
        
        printf("\n-%s:\n", zonas[i].nombre);
        printf("   PM2.5 Promedio: %.2f ug/m3 (Limite OMS: %.1f)\n", zonas[i].promedio_30dias.pm25, LIMITE_PM25);
        printf("   CO2 Promedio:   %.2f ppm    (Limite OMS: %.1f)\n", zonas[i].promedio_30dias.co2, LIMITE_CO2);
    }
}

void predecir_niveles_futuros(ZonaUrbana *zonas) {
    printf("\n  Predicciones de contaminacion para las proximas 24 horas calculadas: \n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        float factor_dispersion = 1.0;
        
        if (zonas[i].clima_actual.velocidad_viento < 8.0) {
            factor_dispersion = 1.25; 
        } else if (zonas[i].clima_actual.velocidad_viento > 15.0) {
            factor_dispersion = 0.85; 
        }
        
        float peso_total = 0;
        float p_pm25 = 0, p_co2 = 0, p_so2 = 0, p_no2 = 0;
        
        for (int d = 0; d < DIAS_HISTORIAL; d++) {
            float peso = (d == DIAS_HISTORIAL - 1) ? 3.0 : 1.0; 
            p_pm25 += zonas[i].historial[d].pm25 * peso;
            p_co2  += zonas[i].historial[d].co2 * peso;
            p_so2  += zonas[i].historial[d].so2 * peso;
            p_no2  += zonas[i].historial[d].no2 * peso;
            peso_total += peso;
        }
        
        zonas[i].prediccion.pm25 = (p_pm25 / peso_total) * factor_dispersion;
        zonas[i].prediccion.co2  = (p_co2 / peso_total) * factor_dispersion;
        zonas[i].prediccion.so2  = (p_so2 / peso_total) * factor_dispersion;
        zonas[i].prediccion.no2  = (p_no2 / peso_total) * factor_dispersion;

        printf("\n-> %s (Prediccion Futura):\n", zonas[i].nombre);
        printf("   PM2.5 Esperado: %.2f ug/m3\n", zonas[i].prediccion.pm25);
        printf("   CO2 Esperado:   %.2f ppm\n", zonas[i].prediccion.co2);
        printf("   SO2 Esperado:   %.2f ug/m3\n", zonas[i].prediccion.so2);
        printf("   NO2 Esperado:   %.2f ug/m3\n", zonas[i].prediccion.no2);
    }
    printf("\n Predicciones de contaminacion para las proximas 24 horas generadas.\n");
}

void evaluar_alertas_y_recomendaciones(ZonaUrbana *zonas) {
    printf("         ALERTAS PREVENTIVAS Y MEDIDAS DE MITIGACION           \n");
    
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        int sobrepasa = 0;
        
        if (zonas[i].prediccion.pm25 > LIMITE_PM25 || zonas[i].actual.pm25 > LIMITE_PM25) {
            printf(" Cuidado. El PM2.5 excede limite OMS (Actual: %.2f, Previsto: %.2f)\n", zonas[i].actual.pm25, zonas[i].prediccion.pm25);
            sobrepasa = 1;
        }
        if (zonas[i].prediccion.co2 > LIMITE_CO2 || zonas[i].actual.co2 > LIMITE_CO2) {
            printf("  Cuidado. El CO2 excede limite OMS (Actual: %.2f, Previsto: %.2f)\n", zonas[i].actual.co2, zonas[i].prediccion.co2);
            sobrepasa = 1;
        }
        if (zonas[i].prediccion.so2 > LIMITE_SO2 || zonas[i].actual.so2 > LIMITE_SO2) {
            printf("  Cuidado. El SO2 excede limite OMS (Actual: %.2f, Previsto: %.2f)\n", zonas[i].actual.so2, zonas[i].prediccion.so2);
            sobrepasa = 1;
        }
        if (zonas[i].prediccion.no2 > LIMITE_NO2 || zonas[i].actual.no2 > LIMITE_NO2) {
            printf("  Cuidado. El NO2 excede limite OMS (Actual: %.2f, Previsto: %.2f)\n", zonas[i].actual.no2, zonas[i].prediccion.no2);
            sobrepasa = 1;
        }
        
        if (sobrepasa) {
            printf("  Recomendaciones de mitigación decretadas:\n");
            printf("     1. Restriccion de circulacion vehicular selectiva (Pico y Placa Ambiental).\n");
            printf("     2. Reduccion programada de la actividad industrial pesada estructural.\n");
            printf("     3. Suspender de inmediato actividades fisicas al aire libre.\n");
        } else {
            printf("  -> Estado normal. Calidad del aire dentro de los parametros seguros de la OMS.\n");
        }
    }
}

void exportar_reporte(ZonaUrbana *zonas) {
    FILE *archivo = fopen("reporte_contaminacion.txt", "w");
    if (archivo == NULL) {
        printf(" No se pudo escribir el archivo de reporte debido a permisos locales.\n");
        return;
    }
    
    fprintf(archivo, "---------------------------------------------------\n");
    fprintf(archivo, "  Reporte de calidad del aire urbano consolidado \n");
    fprintf(archivo, "---------------------------------------------------\n\n");
    
    for (int i = 0; i < NUM_ZONAS; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "---------------------------------------------------------------\n");
        fprintf(archivo, "  Valores Actuales  -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f | NO2: %.2f\n", zonas[i].actual.pm25, zonas[i].actual.co2, zonas[i].actual.so2, zonas[i].actual.no2);
        fprintf(archivo, "  Prediccion 24h    -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f | NO2: %.2f\n", zonas[i].prediccion.pm25, zonas[i].prediccion.co2, zonas[i].prediccion.so2, zonas[i].prediccion.no2);
        fprintf(archivo, "  Promedio Hist.    -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f | NO2: %.2f\n\n", zonas[i].promedio_30dias.pm25, zonas[i].promedio_30dias.co2, zonas[i].promedio_30dias.so2, zonas[i].promedio_30dias.no2);
    }
    
    fclose(archivo);
    printf("\n Datos exportados con éxito en 'reporte_contaminacion.txt'.\n");
}

void mostrar_menu(ZonaUrbana *zonas) {
    int opcion;
    int datos_ingresados = 0;
    
    do {
        printf("\n - Menu de control ambiental -\n");
        printf("1. Ingresar datos \n");
        printf("2. Calcular Promedios Historicos\n");
        printf("3. Generar Prediccion para las proximas 24 horas\n");
        printf("4. Evaluar Alertas y Recomendaciones\n");
        printf("5. Exportar Reporte a Archivo Fisico\n");
        printf("6. Salir\n");
        printf("Seleccionar una opcion: ");
        scanf("%d", &opcion);
        
        if (opcion >= 2 && opcion <= 5 && !datos_ingresados) {
            printf("\n Debe registrar las variables manualmente (Opcion 1) antes de operar el sistema.\n");
            continue;
        }
        
        switch (opcion) {
            case 1: 
                ingresar_datos_manual(zonas); 
                datos_ingresados = 1; 
                break;
            case 2: calcular_promedios_historicos(zonas); break;
            case 3: predecir_niveles_futuros(zonas); break;
            case 4: evaluar_alertas_y_recomendaciones(zonas); break;
            case 5: exportar_reporte(zonas); break;
            case 6: printf("Conexión finalizada y recursos liberados.\n"); break;
            default: printf("La opcion no es valida. Intentar de nuevo.\n");
        }
    } while (opcion != 6);
}
