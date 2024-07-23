# Bayesian Spam Filter Project

## Overview
This project implements a Bayesian spam filter in C. It utilizes a Naive Bayes classifier to distinguish between spam and ham (non-spam) emails. The classifier is trained on a dataset of emails, learning to predict the category of new emails based on their content.

## Structure
The project is organized as follows:
- `bayes.c` and `bayes.h`: Implementation of the Bayesian spam filter.
- `filereader.c` and `filereader.h`: Utilities for reading email data from files.
- `hashtable.c` and `hashtable.h`: Implementation of a hash table for efficient data management.
- `main.c`: The main entry point of the application.
- `data/`: Directory containing the dataset of emails, divided into `custom/`, `train/`, and `test/` subdirectories for custom testing, training, and testing datasets, respectively.
- `makefile` and `makefile.win`: Makefiles for building the project on Unix-like and Windows systems, respectively.
- `readme.md`: This file.
- `dokumentace.pdf`: Project documentation written in Czech language