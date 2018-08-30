# Test fichier pour traduire EE

args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)!=3) {
	# args[1] <- "/srv/nfsroot/root/EmptyEpsilon/translation"
	# args[2] <- "/srv/nfsroot/root/EmptyEpsilon"
	# args[3] <- "/srv/nfsroot/root/SeriousProton"
	args[1] <- "/root/EmptyEpsilon/translation/translation"
	args[2] <- "/root/EmptyEpsilon"
	args[3] <- "/root/SeriousProton"
}

dir_dict <- args[1]
dir_EE <- args[2]
dir_SP <- args[3] 



setwd(dir_dict)
Traduction_fr <- read.table("dict_fr.txt",header = TRUE,sep=",",stringsAsFactors=FALSE)

translate <- function(){
	
	setwd(dir_EE)
	liste <- list.files(path = "./src/", pattern = NULL, all.files = FALSE,
		   full.names = TRUE, recursive = TRUE,
		   ignore.case = FALSE, include.dirs = FALSE, no.. = FALSE)

	 liste <- liste[-grep('hardware',liste)]

	file_log = paste("log_translate",format(Sys.time(), "%d-%m-%g_%H-%M"))

	line_log = paste("Début du log : ",Sys.time())

	write(line_log,file=file_log)
	write(" ",file=file_log,append=TRUE)

	for (f in liste){

		text_f <- readLines(f)
		text_f_old <- readLines(f)
		modif_f <- 0
	
		for (id_mot in 1:nrow(Traduction_fr)){
	
			detect_mot <- grep(paste("\"",Traduction_fr[id_mot,1],"\"",sep=''), text_f )
		
			if (length(detect_mot) != 0){
				line_log = paste("Fichier ",f,", Remplacement du mot ",Traduction_fr[id_mot,1],", ligne ",detect_mot,sep="")
				write(line_log,file=file_log,append=TRUE)
		
				text_f <- gsub(paste("\"",Traduction_fr[id_mot,1],"\"",sep=''),paste("\"",Traduction_fr[id_mot,2],"\"",sep=''), text_f )
			
				modif_f <- 1
			}
		}
	
		# S'il y a eu une modif au moins dans le fichier
		if (modif_f == 1){

			# Ecraser avec le nouveau fichier	
				cat(text_f, file=f, sep="\n")

			# Sauvegarder l'ancien fichier en .old
				# f_old <- paste(f,".old",sep="") 
				# cat(text_f_old, file=f_old, sep="\n")

			write(" ",file=file_log,append=TRUE)	
		}
	}
	line_log = paste("Fin du log : ",Sys.time())
	write(line_log,file=file_log,append=TRUE)
}

translate()

translate_SP <- function(){

	setwd(dir_SP)
	liste <- list.files(path = "./src/", pattern = NULL, all.files = FALSE,
		   full.names = TRUE, recursive = TRUE,
		   ignore.case = FALSE, include.dirs = FALSE, no.. = FALSE)

	file_log = paste("log_translate_SP",format(Sys.time(), "%d-%m-%g_%H-%M"))

	line_log = paste("Début du log : ",Sys.time())

	write(line_log,file=file_log)
	write(" ",file=file_log,append=TRUE)

	for (f in liste){

		text_f <- readLines(f)
		text_f_old <- readLines(f)
		modif_f <- 0
	
		for (id_mot in 1:nrow(Traduction_fr)){
	
			detect_mot <- grep(paste("\"",Traduction_fr[id_mot,1],"\"",sep=''), text_f )
		
			if (length(detect_mot) != 0){
				line_log = paste("Fichier ",f,", Remplacement du mot ",Traduction_fr[id_mot,1],", ligne ",detect_mot,sep="")
				write(line_log,file=file_log,append=TRUE)
		
				text_f <- gsub(paste("\"",Traduction_fr[id_mot,1],"\"",sep=''),paste("\"",Traduction_fr[id_mot,2],"\"",sep=''), text_f )
			
				modif_f <- 1
			}
		}
	
		# S'il y a eu une modif au moins dans le fichier
		if (modif_f == 1){

			# Ecraser avec le nouveau fichier	
				cat(text_f, file=f, sep="\n")

			# Sauvegarder l'ancien fichier en .old
				f_old <- paste(f,".old",sep="") 
				cat(text_f_old, file=f_old, sep="\n")

			write(" ",file=file_log,append=TRUE)	
		}
	}
	line_log = paste("Fin du log : ",Sys.time())
	write(line_log,file=file_log,append=TRUE)
}

translate_SP()
