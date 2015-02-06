package org.semanticweb.HermiT.examples;

import java.io.BufferedOutputStream;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintWriter;

import org.semanticweb.HermiT.Reasoner;
import org.semanticweb.owlapi.apibinding.OWLManager;
import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.util.OWLOntologyMerger;
import org.semanticweb.owlapi.*;
import org.semanticweb.HermiT.examples.DLFileProcess;

public class OntologyToDatalogPrinting {

	public static void main(String[] args) throws Exception {
    	// First, we create an OWLOntologyManager object. The manager will load and
    	// save ontologies.
        OWLOntologyManager manager=OWLManager.createOWLOntologyManager();
        // Now, we create the file from which the ontology will be loaded.
    	// Here the ontology is stored in a file locally in the ontologies subfolder
    	// of the examples folder.
        String source_url = "examples/lubm-full/";
        File inputOntologyFile = new File(source_url+"univ-bench.owl");
        OWLOntology Tbox = manager.loadOntologyFromOntologyDocument(inputOntologyFile);
        for (int i = 0; i < 15; i++) {
        	String aboxfilename = source_url+"University0_"+i+".owl";
        	File inputAboxFile = new File(aboxfilename);
            OWLOntology Abox = manager.loadOntologyFromOntologyDocument(inputAboxFile);
        }
        // We use the OWL API to load the ontology.
        OWLOntologyMerger merger = new OWLOntologyMerger(manager);
        IRI mergedOntologyIRI = IRI.create("http://www.semanticweb.com/mymergedont");
        OWLOntology ontology = merger.createMergedOntology(manager, mergedOntologyIRI);
        System.out.println(ontology.toString());
        // Now we can start and create the reasoner. Here we create an instance of HermiT
        // without any particular configuration given, which means HermiT uses default
        // parameters for blocking etc.
	    Reasoner hermit = new Reasoner(ontology);
	    
	    File DLFile = new File(source_url+"lubm-univ");
	    if (DLFile.exists()) {
	    	DLFile.delete();
	    }
	    DLFile.createNewFile();
	    
	    FileOutputStream out=new FileOutputStream(DLFile,true);     
	    out.write(hermit.getDLOntology().toString().getBytes());
	    out.close();
	    DLFileProcess dlp = new DLFileProcess();
	    dlp.process_dl(source_url);
	}
}
