#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ZONAS 5
#define DIAS_HISTORIAL 30

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

void inicializar_datos(ZonaUrbana *zonas);
void calcular_promedios_historicos(ZonaUrbana *zonas);
void predecir_niveles_futuros(ZonaUrbana *zonas);
void evaluar_alertas_y_recomendaciones(ZonaUrbana *zonas);
void exportar_reporte(ZonaUrbana *zonas);
void mostrar_menu(ZonaUrbana *zonas);

int main() {
    ZonaUrbana zonas[NUM_ZONAS];
    
    printf(" Sistema integral de gestion y prediccion de contaminacion del aire \n");
    inicializar_datos(zonas);
    mostrar_menu(zonas);
    
    return 0;
}

void inicializar_datos(ZonaUrbana *zonas) {
    char *nombres_semilla[NUM_ZONAS] = {
        "Zona Norte (Industrial)", 
        "Zona Sur (Residencial)", 
        "Centro Historico", 
        "Zona Este (Comercial)", 
        "Zona Oeste (Periferia)"
    };
    
    for (int i = 0; i < NUM_ZONAS; i++) {
        strcpy(zonas[i].nombre, nombres_semilla[i]);
        
        zonas[i].clima_actual.temperatura = 18.0 + (i * 1.5);
        zonas[i].clima_actual.velocidad_viento = 12.0 - (i * 1.8);
        zonas[i].clima_actual.humedad = 65.0 + (i * 2.0);
        
        
        zonas[i].actual.pm25 = 12.0 + (i * 8.5);
        zonas[i].actual.co2  = 350.0 + (i * 35.0);
        zonas[i].actual.so2  = 15.0 + (i * 12.0);
        zonas[i].actual.no2  = 20.0 + (i * 5.5);
        
        
        for (int d = 0; d < DIAS_HISTORIAL; d++) {
            zonas[i].historial[d].pm25 = zonas[i].actual.pm25 * (0.85 + ((float)(d % 5) / 10.0));
            zonas[i].historial[d].co2  = zonas[i].actual.co2  * (0.90 + ((float)(d % 4) / 10.0));
            zonas[i].historial[d].so2  = zonas[i].actual.so2  * (0.80 + ((float)(d % 6) / 10.0));
            zonas[i].historial[d].no2  = zonas[i].actual.no2  * (0.88 + ((float)(d % 3) / 10.0));
        }
    }
    printf(" Se inicio completante. \n");
}

void calcular_promedios_historicos(ZonaUrbana *zonas) {
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
    }
    printf("\n>> PROMEDIOS HISTORICOS DE LOS ULTIMOS 30 DIAS CALCULADOS <<\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        printf("-%s -> PM2.5 Promedio: %.2f ug/m3 (Limite OMS: %.1f)\n", 
               zonas[i].nombre, zonas[i].promedio_30dias.pm25, LIMITE_PM25);
    }
}

void predecir_niveles_futuros(ZonaUrbana *zonas) {

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
            float peso = (d >= DIAS_HISTORIAL - 5) ? 3.0 : 1.0;
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
    }
    printf("\n[INFO] Predicciones de contaminacion para las proximas 24 horas generadas.\n");
}

void evaluar_alertas_y_recomendaciones(ZonaUrbana *zonas) {
    printf("\n Alertas preventivas y medidas de mitigación \n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        int sobrepasa = 0;
        
        if (zonas[i].prediccion.pm25 > LIMITE_PM25 || zonas[i].actual.pm25 > LIMITE_PM25) {
            printf(" PM2.5 excede limite de la OMS (Actual: %.2f, Previsto: %.2f)\n", zonas[i].actual.pm25, zonas[i].prediccion.pm25);
            sobrepasa = 1;
        }
        if (zonas[i].prediccion.co2 > LIMITE_CO2) {
            printf(" Niveles de CO2 altos en prediccion (Previsto: %.2f)\n", zonas[i].prediccion.co2);
            sobrepasa = 1;
        }
        
        if (sobrepasa) {
            printf(" Recomendaciones de mitigación:\n");
            printf("Restriccion de circulacion vehicular selectiva (Pico y Placa).\n");
            printf("Reduccion de la actividad industrial pesada en la periferia.\n");
            printf("Suspender actividades fisicas e institucionales al aire libre.\n");
        } else {
            printf(" Estado normal. Calidad del aire dentro de los parametros seguros.\n");
        }
    }
}

void exportar_reporte(ZonaUrbana *zonas) {
    FILE *archivo = fopen("reporte_contaminacion.txt", "w");
    if (archivo == NULL) {
        printf("No se pudo escribir el archivo de reporte.\n");
        return;
    }
    
    fprintf(archivo, "---------------------------------------------------\n");
    fprintf(archivo, "  Reporte de calidad del aire urbano \n");
    fprintf(archivo, "---------------------------------------------------\n\n");
    
    for (int i = 0; i < NUM_ZONAS; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "---------------------------------------------------------------\n");
        fprintf(archivo, "  Valores Actuales  -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f\n", zonas[i].actual.pm25, zonas[i].actual.co2, zonas[i].actual.so2);
        fprintf(archivo, "  Prediccion 24h    -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f\n", zonas[i].prediccion.pm25, zonas[i].prediccion.co2, zonas[i].prediccion.so2);
        fprintf(archivo, "  Promedio 30 dias  -> PM2.5: %.2f | CO2: %.2f | SO2: %.2f\n\n", zonas[i].promedio_30dias.pm25, zonas[i].promedio_30dias.co2, zonas[i].promedio_30dias.so2);
    }
    
    fclose(archivo);
    printf("\n Datos guardados en 'reporte_contaminacion.txt'.\n");
}

void mostrar_menu(ZonaUrbana *zonas) {
    int opcion;
    do {
        printf("\n - Menu de control ambiental -\n");
        printf("1. Calcular Promedios Historicos (30 dias)\n");
        printf("2. Generar Prediccion para las proximas 24 horas\n");
        printf("3. Evaluar Alertas y Recomendaciones\n");
        printf("4. Exportar Reporte a Archivo Físico\n");
        printf("5. Salir\n");
        printf("Seleccionar una opcion: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1: calcular_promedios_historicos(zonas); break;
            case 2: predecir_niveles_futuros(zonas); break;
            case 3: evaluar_alertas_y_recomendaciones(zonas); break;
            case 4: exportar_reporte(zonas); break;
            case 5: printf("Conexión finalizada.\n"); break;
            default: printf("La opcion no es valida. Intentar de nuevo.\n");
        }
    } while (opcion != 5);
}
