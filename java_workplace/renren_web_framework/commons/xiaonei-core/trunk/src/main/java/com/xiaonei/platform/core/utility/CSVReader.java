package com.xiaonei.platform.core.utility;

import java.io.BufferedReader;
import java.io.EOFException;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;

/**
 * Read  CSV (Comma Separated Value)
 * files.
 *
 * This format is used my Microsoft Word and Excel.
 * Fields are separated by commas, and enclosed in
 * quotes if they contain commas or quotes.
 * Embedded quotes are doubled.
 * Embedded spaces do not normally require surrounding quotes.
 * The last field on the line is not followed by a comma.
 * Null fields are represented by two commas in a row.
 * We ignore leading and trailing spaces on fields, even inside quotes.
 * File must normally end with a single CrLf, other wise you will get a null
 * when trying to read a field.
 *
 * @author copyright (c) 2002-2004 Roedy Green  Canadian Mind Products
 * @version 1.0 2002 March 27
 *          1.1 2002 March 28
 *              - close
 *              - configurable separator char
 *              - no longer sensitive to line-ending convention.
 *              - uses a categorise routine to massage categories for
 *                use in case clauses.
 *              - faster skipToNextLine
 *          1.2 2002 April 23
 *               - put in to separate package
 *          1.4 2002 April 19
 *               - fix bug if last field on line is empty, was not counting as a field.
 *          1.6 2002 May 25
 *               - allow choice of " or ' quote char.
 *          1.7 2002 August 29
 *               - getAllFieldsInLine
 *          1.8 2002 November 12
 *               - allow Microsoft Excel format fields that can span several lines.
 *                 sponsored by Steve Hunter of agilense.com
 *          1.9 2002 November 14
 *               - trim parameter to control whether fields are trimmed of
 *                 lead/trail whitespace (blanks, Cr, Lf, Tab etc.)
 *          2.0 2003 August 10
 *               - getInt, getLong, getFloat, getDouble
 *
 * There is another CSVReader at:
 * at http://ostermiller.org/utils/ExcelCSV.html
 * If this CSVReader is not suitable for you, try that one.
 *
 * Future ideas:
 *
 * 1. allow specify various comment chars that mean the rest of the line should be ignored.
 * e.g. ; ! #.  These chars have to be in quotes in data then.
 *
 * 2. allow \ to be used for quoting characters.
 */


public class CSVReader
{
//	private static final boolean debugging = true;
	
	/**
	 * Reader source of the CSV fields to be read.
	 */
	private BufferedReader r;
	
	/**
	 * field separator character, usually ',' in North America,
	 * ';' in Europe and sometimes '\t' for tab.
	 */
	private final char separator;
	
	
	/**
	 * quote character, usually '\"'
	 * '\'' for SOL used to enclose fields containing a separator character.
	 */
	private final char quote;
	
	/**
	 * true if reader should allow
	 * quoted fields to span
	 * more than one line.  Microsoft Excel
	 * sometimes generates files like this.
	 */
	private final boolean allowMultiLineFields;
	
	/**
	 * true if reader should trim lead/trail whitespace
	 * from fields returned.
	 */
	private final boolean trim;
	
	/**
	 * category of end of line char.
	 */
	private static final int EOL = 0;
	
	/**
	 * category of ordinary character
	 */
	private static final int ORDINARY = 1;
	
	
	/**
	 * categotory of the quote mark "
	 */
	private static final int QUOTE = 2;
	
	/**
	 * category of the separator, e.g. comma, semicolon
	 * or tab.
	 */
	private static final int SEPARATOR = 3;
	
	/**
	 * category of characters treated as white space.
	 */
	private static final int WHITESPACE = 4;
	
	/**
	 * e.g. \n \r\n or \r, whatever system uses to separate lines in a text file.
	 */
	static  String lineSeparator = System.getProperty ( "line.separator" );
	
	static {
		if ( lineSeparator == null )
		{
			lineSeparator = "\015012"; /* crlf in octal */
		}
	}
	
	/**
	 * parser: We are in blanks before the field.
	 */
	private static final int SEEKING_START = 0;
	/**
	 * parser: We are in the middle of an ordinary field.
	 */
	private static final int IN_PLAIN = 1;
	/**
	 * parser: e are in middle of field surrounded in quotes.
	 */
	private static final int IN_QUOTED = 2;
	
	/**
	 * parser: We have just hit a quote, might be doubled
	 * or might be last one.
	 */
	private static final int AFTER_END_QUOTE = 3;
	
	/**
	 * parser: We are in blanks after the field looking for the separator
	 */
	private static final int SKIPPING_TAIL = 4;
	
	/**
	 * The line we are parsing.
	 * null means none read yet.
	 * Line contains unprocessed chars. Processed ones are removed.
	 */
	private String line = null;
	
	/**
	 * How many lines we have read so far.
	 * Used in error messages.
	 */
	private int lineCount = 0;
	
	/**
	 * false means next EOL marks an empty field
	 * true means next EOL marks the end of all fields.
	 */
	private boolean allFieldsDone = true;
	
	/**
	 * state of the parser's finite state automaton.
	 */
	
	/**
	 * convenience Constructor, default to comma separator, " for quote,
	 * no multiline fields, with trimming.
	 *
	 * @param r     input Reader source of CSV Fields to read.
	 */
	public CSVReader (final Reader r)
	{
		this(r, ',', '\"', false, true );
	}
	
	/**
	 * Constructor
	 *
	 * @param r         input Reader source of CSV Fields to read.
	 *
	 * @param separator
	 *                  field separator character, usually ',' in North America,
	 *                  ';' in Europe and sometimes '\t' for tab.
	 * @param quote
	 *                  char to use to enclose fields containing a separator,
	 *                  usually '\"'
	 * @param allowMultiLineFields
	 *                  true if reader should allow
	 *                  quoted fields to span
	 *                  more than one line.  Microsoft Excel
	 *                  sometimes generates files like this.
	 * @param trim      true if reader should trim lead/trailing whitespace
	 *                  e.g. blanks, Cr, Lf. Tab off fields.
	 */
	public CSVReader ( final Reader r,
			final char separator,
			final char quote,
			final boolean allowMultiLineFields,
			final boolean trim
	)
	{
		/* convert Reader to BufferedReader if necessary */
		if ( r instanceof BufferedReader )
		{
			this.r = (BufferedReader) r;
		}
		else
		{
			this.r = new BufferedReader(r);
		}
		if ( this.r == null )
		{
			throw new IllegalArgumentException("invalid Reader");
		}
		
		this.separator = separator;
		this.quote = quote;
		this.allowMultiLineFields = allowMultiLineFields;
		this.trim = trim;
	}
	/**
	 * Close the Reader.
	 */
	public void close() throws IOException
	{
		if ( this.r != null )
		{
			this.r.close();
			this.r = null;
		}
	}
	
	
	/**
	 * Read one field from the CSV file
	 *
	 * @return String value, even if the field is numeric.  Surrounded
	 *         and embedded double quotes are stripped.
	 *         possibly "".  null means end of line.
	 *
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 *
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 */
	public String get() throws EOFException, IOException
	{
		
		final StringBuffer field = new StringBuffer( this.allowMultiLineFields ? 512 : 64 );
		/* we implement the parser as a finite state automaton with five states. */
		
		
		int state = SEEKING_START; /* start seeking, even if partway through a line */
		/* don't need to maintain state between fields. */
		
//		lineLoop:
			while ( true )
			{
				
				this.getLineIfNeeded();
				
				/**
				 * The following sentence convert the 126.com,263.com,tom.com format to the standart format
				 */
				this.line=this.line.replaceAll("%22","\"\"");
				
				charLoop:
					/* loop for each char in the line to find a field */
					/* guaranteed to leave early by hitting EOL */
					for ( int i=0; i<this.line.length(); i++ )
					{
						final char c = this.line.charAt(i);
						final int category = this.categorise(c);
						switch ( state )
						{
						case SEEKING_START:
						{  /* in blanks before field */
							switch ( category )
							{
							case WHITESPACE:
								/* ignore */
								break;
								
							case QUOTE:
								state = IN_QUOTED;
								break;
								
							case SEPARATOR:
								/* end of empty field */
								this.line = this.line.substring(i+1);
								return "";
								
							case EOL:
								/* end of line */
								if ( this.allFieldsDone )
								{
									/* null to mark end of line */
									this.line = null;
									return null;
								}
								else
								{
									/* empty field, usually after a comma */
									this.allFieldsDone = true;
									this.line = this.line.substring(i);
									return "";
								}
								
							case ORDINARY:
								field.append(c);
								state = IN_PLAIN;
								break;
							}
							break;
						}
						case IN_PLAIN:
						{  /* in middle of ordinary field */
							switch ( category )
							{
							case QUOTE:
								throw new IOException("Malformed CSV stream. Missing quote at start of field on line "+this.lineCount);
								
							case SEPARATOR:
								/* done */
								this.line = this.line.substring(i+1);
								return this.maybeTrim( field.toString() );
								
							case EOL:
								this.line = this.line.substring(i); /* push EOL back */
								this.allFieldsDone = true;
								return this.maybeTrim( field.toString() );
								
							case WHITESPACE:
								field.append(c);//原本是' ',我改为c
								break;
								
							case ORDINARY:
								field.append(c);
								break;
							}
							break;
						}
						
						case IN_QUOTED:
						{  /* in middle of field surrounded in quotes */
							switch ( category )
							{
							
							case QUOTE:
								state = AFTER_END_QUOTE;
								break;
								
							case EOL:
								if ( this.allowMultiLineFields )
								{
									field.append( lineSeparator );
									// we are done with that line, but not with the field.
									// We don't want to return a null
									// to mark the end of the line.
									this.line = null;
									// will read next line and seek the end of the quoted field.
									// with state = IN_QUOTED.
									break charLoop;
								}
								else
								{
									// no multiline fields allowed
									this.allFieldsDone = true;
									throw new IOException ("Malformed CSV stream. Missing quote (\") after field on line "+this.lineCount);
								}
							case WHITESPACE:
								field.append(c);//原本是' ',我改为c.
								break;
								
							case SEPARATOR:
							case ORDINARY:
								field.append(c);
								break;
							}
							break;
						}
						
						case AFTER_END_QUOTE:
						{
							
							/* In situation like this "xxx" which may
							turn out to be xxx""xxx" or "xxx",
							We find out here. */
							switch ( category )
							{
							
							case QUOTE:
								/* was a double quote, e.g. a literal " */
								field.append(c);
								state = IN_QUOTED;
								break;
								
							case SEPARATOR :
								/* we are done with field.*/
								this.line = this.line.substring(i+1);
								return this.maybeTrim( field.toString() );
								
							case EOL:
								this.line = this.line.substring(i); /* push back eol */
								this.allFieldsDone = true;
								return this.maybeTrim( field.toString() );
								
							case WHITESPACE:
								/* ignore trailing spaces up to separator */
								state = SKIPPING_TAIL;
								break;
								
							case ORDINARY:
								throw new IOException("Malformed CSV stream, missing separator after fieldon line "+this.lineCount);
							}
							break;
						}
						
						case SKIPPING_TAIL:
						{
							/* in spaces after field seeking separator */
							
							switch ( category )
							{
							
							case SEPARATOR :
								/* we are done.*/
								this.line = this.line.substring(i+1);
								return this.maybeTrim( field.toString() );
								
							case EOL:
								this.line = this.line.substring(i); /* push back eol */
								this.allFieldsDone = true;
								return this.maybeTrim( field.toString() );
								
							case WHITESPACE:
								/* ignore trailing spaces up to separator */
								break;
								
							case QUOTE:
							case ORDINARY:
								throw new IOException("Malformed CSV stream, missing separator after field on line "+this.lineCount);
							}
							break;
						}
						
						} // end switch(state)
						
					} // end charLoop
			} // end lineLoop
	} // end get
	/**
	 * Get all fields in the line
	 *
	 * @return Array of strings, one for each field.
	 *         Possibly empty, but never null.
	 * @exception EOFException
	 * @exception IOException
	 */
	public String[] getAllFieldsInLine() throws EOFException, IOException
	{
		final ArrayList<String> al = new ArrayList<String>( 30 );
		do
		{
			String field=null;
			try {
				field = this.get();
			} catch (final EOFException e1) {
				throw new EOFException();
			} catch (final IOException e) {
				/**
				 * 这样保证即使其中有一行有问题也能继续处理后面的
				 */
				String tmpString;
				tmpString=e.getMessage();
				if ((tmpString != null) && (tmpString.indexOf("Malformed CSV stream") >= 0)) {
					this.line=null;
					al.clear();
				}else {
					throw new IOException();
				}
			}
			if ( field == null )
			{
				break;
			}
			al.add (field);
		}
		while ( true );
		return (al.toArray( new String[ al.size() ] ));
	}
	
	/**
	 * Read one double field from the CSV file.
	 *
	 * @return houble value, empty field returns 0, as does end of line.
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 * @exception NumberFormatException, if field does not contain a well-formed int.
	 */
	public double getDouble() throws EOFException, IOException, NumberFormatException
	{
		String s = this.get();
		if ( s == null )
		{
			return 0;
		}
		if ( ! this.trim )
		{
			s = s.trim();
		}
		if ( s.length() == 0 )
		{
			return 0;
		}
		return Double.parseDouble( s );
	}
	
	/**
	 * Read one float field from the CSV file.
	 *
	 * @return float value, empty field returns 0, as does end of line.
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 * @exception NumberFormatException, if field does not contain a well-formed int.
	 */
	public float getFloat() throws EOFException, IOException, NumberFormatException
	{
		String s = this.get();
		if ( s == null )
		{
			return 0;
		}
		if ( ! this.trim )
		{
			s = s.trim();
		}
		if ( s.length() == 0 )
		{
			return 0;
		}
		return Float.parseFloat( s );
	}
	
	
	/**
	 * Read one integer field from the CSV file
	 *
	 * @return int value, empty field returns 0, as does end of line.
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 * @exception NumberFormatException, if field does not contain a well-formed int.
	 */
	public int getInt() throws EOFException, IOException, NumberFormatException
	{
		String s = this.get();
		if ( s == null )
		{
			return 0;
		}
		if ( ! this.trim )
		{
			s = s.trim();
		}
		if ( s.length() == 0 )
		{
			return 0;
		}
		return Integer.parseInt( s );
	}
	
	
	/**
	 * Read one long field from the CSV file
	 *
	 * @return long value, empty field returns 0, as does end of line.
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 * @exception NumberFormatException, if field does not contain a well-formed int.
	 */
	public long getLong() throws EOFException, IOException, NumberFormatException
	{
		String s = this.get();
		if ( s == null )
		{
			return 0;
		}
		if ( ! this.trim )
		{
			s = s.trim();
		}
		
		if ( s.length() == 0 )
		{
			return 0;
		}
		return Long.parseLong( s );
	}
	
	/**
	 * Skip over fields you don't want to process.
	 *
	 * @param fields How many field you want to bypass reading.
	 *               The newline counts as one field.
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 */
	public void skip(final int fields) throws EOFException, IOException
	{
		
		if ( fields <= 0 )
		{
			return;
		}
		for ( int i=0; i<fields; i++ )
		{
			// throw results away
			this.get();
		}
	}
	
	/**
	 * Skip over remaining fields on this line you don't want to process.
	 *
	 * @exception EOFException
	 *                   at end of file after all the fields have
	 *                   been read.
	 * @exception IOException
	 *                   Some problem reading the file, possibly malformed data.
	 */
	public void skipToNextLine() throws EOFException, IOException
	{
		
		if ( this.line == null )
		{
			this.getLineIfNeeded();
		}
		this.line = null;
	}
	
	/**
	 * categorise a character for the finite state machine.
	 *
	 * @param c      the character to categorise
	 * @return integer representing the character's category.
	 */
	private int categorise ( final char c )
	{
		switch ( c )
		{
		case ' ':
		case '\r':
		case 0xff:
			return WHITESPACE;
			
		case '\n':
			return EOL; /* artificially applied to end of line */
			
			
			
		default:
			if ( c == this.quote )
			{
				return QUOTE;
			}
			else if ( c == this.separator /* dynamically determined so can't use as case label */ )
			{
				return SEPARATOR;
			}
			/* do our tests in crafted order, hoping for an early return */
			else if ( ('!' <= c) && (c <= '~') )
			{
				return ORDINARY;
			}
			else if ( (0x00 <= c) && (c <= 0x20) )
			{
				return WHITESPACE;
			}
			else if ( Character.isWhitespace(c) )//这个没必要吧?
			{
				return WHITESPACE;
			}
			else
			{
				return ORDINARY;
			}
		}
	}
	
	/**
	 * Make sure a line is available for parsing.
	 * Does nothing if there already is one.
	 *
	 * @exception EOFException
	 */
	private void getLineIfNeeded() throws EOFException, IOException
	{
		
		if ( this.line == null )
		{
			if ( this.r == null )
			{
				throw new IllegalArgumentException("attempt to use a closed CSVReader");
			}
			this.allFieldsDone = false;
			this.line = this.r.readLine();  /* this strips platform specific line ending */
			if ( this.line == null )   /* null means EOF, yet another inconsistent Java convention. */
			{
				throw new EOFException();
			}
			else
			{
				this.line += '\n'; /* apply standard line end for parser to find */
				this.lineCount++;
			}
		}
	}
	
	/**
	 * Trim the string, but only if we are in
	 * trimming mode.
	 *
	 * @param s      String to be trimmed.
	 * @return String or trimmed string.
	 */
	private String maybeTrim (final String s )
	{
		if ( this.trim )
		{
			return s.trim();
		}
		else
		{
			return s;
		}
	}
	
//	/**
//	 * Test driver
//	 *
//	 * @param args   not used
//	 */
//	public static void main(String[] args)
//	{
//		if ( debugging )
//		{
//			try
//			{
//				// read test file
//				CSVReader csv = new CSVReader(new FileReader("test.csv"), ',', '\"', true, false);
//				try
//				{
//					while ( true )
//					{
////						println("--> " + csv.get());
//					}
//				}
//				catch ( EOFException  e )
//				{
//				}
//				csv.close();
//			}
//			catch ( IOException  e )
//			{
//				e.printStackTrace();
//				println(e.getMessage());
//			}
//		} // end if
//	} // end main
} // end CSVReader class.

