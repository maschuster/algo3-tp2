library(tidyverse)

TSCE <- read_csv("TSCE.csv", col_types = cols(Mejora = col_logical())) %>%
  mutate(
    Algoritmo = "TSC-E",
    Aspirar = T,
    Iteracion = row_number()
  )

TSCC <- read_csv("TSCC.csv", col_types = cols(Mejora = col_logical())) %>%
  mutate(Algoritmo = "TSC-C",
         Aspirar = T,
         Iteracion = row_number()
  ) %>%
  filter(Iteracion <= 200)

TSCE_noA <- read_csv("TSCE_noA.csv", col_types = cols(Mejora = col_logical())) %>%
  mutate(
    Algoritmo = "TSC-E",
    Aspirar = F,
    Iteracion = row_number()
  )

TSCC_noA <- read_csv("TSCC_noA.csv", col_types = cols(Mejora = col_logical())) %>%
  mutate(Algoritmo = "TSC-C",
         Aspirar = F,
         Iteracion = row_number()
  ) %>%
  filter(Iteracion <= 200)

data <- bind_rows(
  TSCE,
  TSCC,
  TSCE_noA,
  TSCC_noA
) %>%
  mutate(
    Memoria = substr(Algoritmo, 5, 5),
    Gap = (47-Impacto)/47
  )

ggplot(data, aes(x = Iteracion, y = Impacto, group = Aspirar, color = Aspirar, fill = Mejora)) +
  geom_hline(yintercept = 47, linetype = "dashed", alpha = .5) +
  geom_line() +
  geom_point(shape = 21) +
  facet_wrap(~Algoritmo, scale = "free_x", ncol = 1) +
  scale_fill_manual(values = c("#FFFFFF", "#000000")) +
  theme(legend.position = "bottom")

ggsave("plots/impacto_tiempo.png", width = 8, height = 7)

# ggplot(data, aes(x = Iteracion, y = Gap, group = Algoritmo, color = Algoritmo, fill = Mejora)) +
#   geom_line() +
#   geom_point(shape = 21) +
#   facet_wrap(~Memoria, scale = "free_x", ncol = 1) +
#   scale_fill_manual(values = c("#FFFFFF", "#000000"))
