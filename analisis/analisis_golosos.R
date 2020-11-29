library(tidyverse)
library(patchwork)

data <- read_csv("../notebooks/goloso_opt_total.csv")

g1 <- data %>%
  group_by(Inicial, N) %>%
  summarize(
    meanGap = mean(Gap),
    seGap = sd(Gap)/sqrt(n())
  ) %>%
  ggplot(aes(x = N, y = meanGap, color = Inicial, group = Inicial)) +
  geom_errorbar(aes(ymin = meanGap - seGap, ymax = meanGap + seGap), width = 0.5) +
  geom_line() +
  geom_point() +
  ylab("Gap promedio") +
  xlab("Tamaño de la instancia") +
  labs(color='Heurística') +
  theme(legend.position = "none")
  

g2 <- data %>%
  group_by(Inicial, N) %>%
  summarize(
    meanTime = mean(Time),
    seTime = sd(Time)/sqrt(n())
  ) %>%
  ggplot(aes(x = N, y = meanTime, color = Inicial, group = Inicial)) +
  geom_errorbar(aes(ymin = meanTime - seTime, ymax = meanTime + seTime), width = 0.5) +
  geom_line() +
  geom_point() +
  ylab("Tiempo promedio (ms)") +
  xlab("Tamaño de la instancia") +
  labs(color='Heurística') 

g1 + g2

ggsave("plots/heuristicas_constructivas.png", width = 10, height = 5)
