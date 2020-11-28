library(tidyverse)
library(pheatmap)

data <- read_csv("../notebooks/tabu_opt_total-3it.csv") %>%
  mutate(memoriaEfectiva = 
    case_when(
      CantMemoria >= ItEfectivas ~ ItEfectivas,
      TRUE                       ~ CantMemoria
    ))

dataTest  <- read_csv("../notebooks/tabu_test_limited_total.csv") %>%
  mutate(memoriaEfectiva = 
           case_when(
             CantMemoria >= ItEfectivas ~ ItEfectivas,
             TRUE                       ~ CantMemoria
           ))

## Heatpmap individual ---------------------------------------------------------
data_h <- data %>%
  filter(N == 30, Algoritmo == "TSC-C-S-LF", Aspirar, PorcentajeVecindad == 60) %>%
  select(CantMemoria, Iteraciones, Gap, Impacto, Optimo) %>%
  group_by(CantMemoria, Iteraciones) %>%
  summarise(
    meanGap = mean(Gap)
  ) %>%
  spread(Iteraciones, meanGap) %>%
  ungroup()

data_h_m <- as.matrix(data_h %>% select(-CantMemoria))

rownames(data_h_m) <- data_h %>% dplyr::select(CantMemoria) %>% pull()

#data_h_m <- t(scale(t(data_h_m)))

pheatmap(mat = data_h_m, cluster_rows = F, cluster_cols = F)




## Ranking ---------------------------------------------------------------------
ranking <- data %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n()),
    meanItEfectivas = mean(ItEfectivas),
    meanMemoriaEfectiva = mean(memoriaEfectiva)
  ) %>%
  arrange(meanGap) %>%
  group_by(Tabu, Inicial) %>%
  filter(row_number()==1) %>%
  ungroup() %>%
  mutate(Entrenamiento = row_number())

rankingTest <- dataTest %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n()),
    meanItEfectivas = mean(ItEfectivas),
    meanMemoriaEfectiva = mean(memoriaEfectiva)
  )

rankingFull <- left_join(ranking, rankingTest,
          by = c("Tabu", "Inicial", "Iteraciones", "PorcentajeVecindad", "CantMemoria", "Aspirar"),
          suffix = c("_train", "_test")) %>%
  arrange(meanGap_test) %>%
  mutate(Testeo = row_number()) %>%
  gather("Datos", "Orden", c(Entrenamiento, Testeo)) %>%
  mutate(meanGap = 
    case_when(
      Datos == "Entrenamiento" ~ meanGap_train,
      TRUE                     ~ meanGap_test
    )
  )

ggplot(rankingFull,
  aes(y = Orden, x = Datos, label = paste0(Tabu, " ", Inicial, "\n", round(meanGap,3)),
      group = paste(Tabu, Inicial), color = paste(Tabu, Inicial))) +
  geom_line() +
  geom_label() +
  scale_x_discrete(expand = c(0.1, 0.1)) +
  scale_y_continuous(trans = "reverse", breaks = seq(1, nrow(ranking)), expand = c(0.1, 0.1)) +
  theme_minimal() +
  theme(
    panel.grid.major = element_blank(),
    panel.grid.minor = element_blank(),
    legend.position = "none"
    )

# Tenemos la mejor combinacion de argumentos para cada algoritmo en las instancias
# de entrenamiento.
# Tenemos un ranking de algoritmos en las instancias de entrenamiento.

## Histograma ------------------------------------------------------------------
data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n()),
    meanItEfectivas = mean(ItEfectivas),
    meanMemoriaEfectiva = mean(memoriaEfectiva)
  ) %>%
  ggplot(aes(meanGap)) +
  geom_histogram() +
  xlim(0,1) +
  facet_grid(Tabu~Inicial, scale="free_y")

data %>%
  filter(substr(Tabu, 1, 3) == "TSS") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n()),
    meanItEfectivas = mean(ItEfectivas),
    meanMemoriaEfectiva = mean(memoriaEfectiva)
  ) %>%
  ggplot(aes(meanGap)) +
  geom_histogram() +
  xlim(0,1) +
  facet_grid(Tabu~Inicial, scale="free_y")

## Porcentaje de vecindad ------------------------------------------------------
data %>%
  mutate(algo3 = substr(Tabu, 1, 3)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(algo3, Algoritmo, PorcentajeVecindad) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  ggplot(aes(x = PorcentajeVecindad, y = meanGap, group = Algoritmo, color = Algoritmo)) +
  geom_errorbar(aes(ymax = meanGap + seGap, ymin = meanGap - seGap), width = 0.2) +
  geom_point() +
  geom_line() +
  facet_wrap(~algo3, scale = "free_y")

## Efecto de aspirar -----------------------------------------------------------
data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Aspirar) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = Aspirar, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")

data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Aspirar) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = Aspirar, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")

# Antes de esto nos gustaria mostrar de alguna forma que porcentaje de vecindad
# es lo que mas afecta a meanGap
# Aspirar no vale la pena ser variado, se puede dejar siempre activado

## Impacto de activar aspiracion sobre el tiempo -------------------------------
data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanTime = mean(Time/1000),
    seTime = sd(Time/1000)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Aspirar) %>%
  summarize(
    seTime = sd(meanTime)/sqrt(n()),
    meanTime = mean(meanTime)
  ) %>%
  ggplot(aes(y = meanTime, x = Aspirar, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanTime + seTime, ymin = meanTime - seTime), width = 0.1) +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")


data %>%
  filter(substr(Tabu, 1, 3) == "TSS") %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanTime = mean(Time/1000),
    seTime = sd(Time/1000)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Aspirar) %>%
  summarize(
    seTime = sd(meanTime)/sqrt(n()),
    meanTime = mean(meanTime)
  ) %>%
  ggplot(aes(y = meanTime, x = Aspirar, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanTime + seTime, ymin = meanTime - seTime), width = 0.1) +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")
# Por activarlo, estamos pagando ~2 segundos como mucho pero solo en las instancias
# que mejora el resultado. Aceptamos pagarlo.

## Correlacion meanGap vs parametros -------------------------------------------
dataItEfTSC <- data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  filter(Aspirar == TRUE) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanItEfectivas = mean(ItEfectivas),
    seItEfectivas = sd(ItEfectivas)
  ) %>%
  group_by(Tabu, Inicial) %>%
  summarize(
    seItEfectivas = sd(meanItEfectivas),
    meanItEfectivas = mean(meanItEfectivas)
  )

dataItEfTSS <- data %>%
  filter(substr(Tabu, 1, 3) == "TSS") %>%
  filter(Aspirar == TRUE) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanItEfectivas = mean(ItEfectivas),
    seItEfectivas = sd(ItEfectivas)
  ) %>%
  group_by(Tabu, Inicial) %>%
  summarize(
    seItEfectivas = sd(meanItEfectivas),
    meanItEfectivas = mean(meanItEfectivas)
  )


data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  filter(Aspirar == TRUE) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, CantMemoria) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = CantMemoria, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas), color = "black", alpha = 0.5) +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas + seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas - seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")


data %>%
  filter(substr(Tabu, 1, 3) == "TSS") %>%
  filter(Aspirar == TRUE) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  filter(PorcentajeVecindad %in% c(5, 20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, CantMemoria) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = CantMemoria, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas), color = "black", alpha = 0.5) +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas + seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas - seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_point() +
  geom_line() +
  facet_grid(Tabu~Inicial, scales = "free_y")

# La performance se estabiliza a partir de 200. Esto es porque la memoria no se
# esta usando.
# A partir de aca cortamos memoria por debajo de 200.

data %>%
  filter(substr(Tabu, 1, 3) == "TSC") %>%
  filter(Aspirar == TRUE, CantMemoria <= 200) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)
  ) %>%
  filter(PorcentajeVecindad %in% c(20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Iteraciones) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = Iteraciones, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_point() +
  geom_line() +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas), color = "black", alpha = 0.5) +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas + seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_vline(data = dataItEfTSC, aes(xintercept = meanItEfectivas - seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  facet_grid(Tabu~Inicial, scales = "free_y")

data %>%
  filter(substr(Tabu, 1, 3) == "TSS") %>%
  filter(Aspirar == TRUE, CantMemoria <= 200) %>%
  group_by(Tabu, Inicial, Iteraciones, PorcentajeVecindad, CantMemoria, Aspirar) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)
  ) %>%
  filter(PorcentajeVecindad %in% c(20, 40, 60, 80, 100)) %>%
  mutate(PorcentajeVecindad = as.factor(PorcentajeVecindad)) %>%
  group_by(Tabu, Inicial, PorcentajeVecindad, Iteraciones) %>%
  summarize(
    seGap = sd(meanGap)/sqrt(n()),
    meanGap = mean(meanGap)
  ) %>%
  ggplot(aes(y = meanGap, x = Iteraciones, group = PorcentajeVecindad, color = PorcentajeVecindad)) +
  geom_errorbar(aes(ymax= meanGap + seGap, ymin = meanGap - seGap), width = 0.1) +
  geom_point() +
  geom_line() +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas), color = "black", alpha = 0.5) +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas + seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  geom_vline(data = dataItEfTSS, aes(xintercept = meanItEfectivas - seItEfectivas), color = "blue", alpha = 0.5, linetype = "dashed") +
  facet_grid(Tabu~Inicial, scales = "free_y")

# No tiene sentido usar muchas iteraciones porque no las hace
# Aca podriamos definir la cantidad de itreaciones para cada combinacion
