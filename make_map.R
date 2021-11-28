library(leaflet)
library(htmlwidgets)
args <- commandArgs(trailingOnly = TRUE)
if (length(args) != 2) {
    stop("Please specify the node IDs as arguments", call. = FALSE)
} else {
    start <- args[1]
    end <- args[2]
}
df <- read.csv(sprintf("results/spain_%s_%s.csv", start, end))
m <- leaflet(data = df) %>% addTiles() %>% addPolylines(lat = ~lat, lng = ~lon)
outname <- sprintf("results/spain_%s_%s.html", start, end)
title <- sprintf("A*_%s_%s.html", start, end)
saveWidget(m, file = outname, title = title)
