package org.semanticweb.HermiT.examples;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class DLFileProcess {
	public void process_dl(String source_url) throws Exception {
		File dlFile = new File(source_url+"lubm-univ");
		File dlpFile = new File(source_url+"lubm-dlp");
		File nlFile = new File(source_url+"negative_inclu");
		//File aboxFile = new File("examples/Lubm/lubm-abox");
		if (!dlFile.exists()) {
			System.exit(0);
		}
		if (dlpFile.exists()) {
			dlpFile.delete();
		}
//		if (aboxFile.exists()) {
//			aboxFile.delete();
//		}
		dlpFile.createNewFile();
		System.out.println(dlpFile.exists());

		//aboxFile.createNewFile();
		
		BufferedReader reader = new BufferedReader(new FileReader(dlFile));
		BufferedWriter dlpwriter = new BufferedWriter(new FileWriter(dlpFile));
		//FileOutputStream aboxwriter=new FileOutputStream(aboxFile,true); 
		int Process_State = -1;
		String line = null;
		int p_index = 0;
		
		while ((line = reader.readLine()) != null) {
			Pattern pattern;
			Matcher matcher;

			if (isNextPart(line)) {
				Process_State++;
				if (Process_State == 2) {
					BufferedReader n_reader = new BufferedReader(new FileReader(nlFile));
					String n_line = null;
					while ((n_line = n_reader.readLine()) != null) {
						pattern = Pattern.compile("([A-W]|[Z])");
						matcher = pattern.matcher(n_line);
						while (matcher.find()) {
							n_line = matcher.replaceFirst(matcher.group(1).toLowerCase());
							matcher = pattern.matcher(n_line);
						}
						n_line = n_line.substring(0, n_line.length()-1) + ".\r\n";
						dlpwriter.write(n_line);
					}
//					n_line = "p" + 0;
//					for (int i = 1; i < p_index; i++) {
//						n_line += " | p" + i;
//					}
//					n_line += ".\r\n";
//					dlpwriter.write(n_line);
					n_reader.close();
				}
				continue;
			}
			if (line.contains("[") || line.contains("]")) {
				continue;
			}				
			
			if (Process_State == 1) {
				pattern = Pattern.compile("<(http://){1}\\S+#([A-Za-z0-9]*)>");
				matcher = pattern.matcher(line);
				while (matcher.find()) {
					line = matcher.replaceFirst(matcher.group(2));
					matcher = pattern.matcher(line);
				}
				pattern = Pattern.compile("([A-W]|[Z])");
				matcher = pattern.matcher(line);
				while (matcher.find()) {
					line = matcher.replaceFirst(matcher.group(1).toLowerCase());
					matcher = pattern.matcher(line);
				}
				pattern = Pattern.compile("<internal:all#([A-Za-z0-9]*)>");
				matcher = pattern.matcher(line);
				while (matcher.find()) {
					line = matcher.replaceFirst("internal_all_"+matcher.group(1));
					matcher = pattern.matcher(line);
				}
				if (line.contains("atleast")) {
					String al[] = line.split(" ");
					String pre_1 = al[3];
					String pre_2 = al[4].split("\\)")[0];
					String tailer = al[5]+" "+al[6];
					if (pre_1.subSequence(0, 3).equals("inv")) {
						line = pre_1.subSequence(4, pre_1.length()-1).toString()+"(f(X),X) " + tailer + ".\r\n"+pre_2+"(f(X)) "+tailer;
					} else {
						line = pre_1+"(X,f(X)) " + tailer + ".\r\n" +pre_2+"(f(X)) "+tailer;
					}
				} else {
					line = line.substring(2, line.length());
				}
				if (line.contains("owl:")) {
					line = line.replace("owl:", "");
				}
				line = line + ".\r\n";
				System.out.println(line);
				dlpwriter.write(line);
			} else if (Process_State == 2) {
				pattern = Pattern.compile("<\\S+#([A-Za-z0-9]*)>");
				matcher = pattern.matcher(line);
				if (matcher.find()) {
					line = matcher.replaceFirst(matcher.group(1));
				}
				pattern = Pattern.compile("<(http://www.){1}(\\S+\\.)+(edu){1}(/\\S+)*>");
				matcher = pattern.matcher(line);
				StringBuffer replacedStr = new StringBuffer();
				if (matcher.find()) {
					replacedStr.append(matcher.group(2));
					String str = replacedStr.toString().replace(".", "_");
					str = str.subSequence(0, str.length()-1).toString();
					if (matcher.group(4) != null) {
						str = str + matcher.group(4);
						str = str.replace("/", "_");
					}
					line = matcher.replaceAll(str);
				}
				pattern = Pattern.compile("([A-W]|[Z])");
				matcher = pattern.matcher(line);
				while (matcher.find()) {
					line = matcher.replaceFirst(matcher.group(1).toLowerCase());
					matcher = pattern.matcher(line);
				}
				pattern = Pattern.compile("\\((\\S+)\\)");
				matcher = pattern.matcher(line);
				if (matcher.find()) {
					line = matcher.replaceAll("("+matcher.group(1).toLowerCase()+")");
				}
				if (line.contains(":")) {
					line.replace(":", "_");
				}
				line = line.substring(2, line.length()) + ".\r\n";
				dlpwriter.write(line);
			}
		}
		dlpwriter.close();
		reader.close();
	}
	
	private boolean isNextPart(String line) {
		if (line.contains("Prefixes") || line.contains("Deterministic DL-clauses:") 
				|| line.contains("ABox") || line.contains("Statistics")) {
			return true;
		}
		return false;
	}
}
