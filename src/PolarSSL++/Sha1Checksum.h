
// Sha1Checksum.h

// Declares the cSha1Checksum class representing the SHA-1 checksum calculator





#pragma once

#include "polarssl/sha1.h"





/** Calculates a SHA1 checksum for data stream */
class cSha1Checksum
{
public:
	typedef Byte Checksum[20];  // The type used for storing the checksum
	
	cSha1Checksum(void);
	
	/** Adds the specified data to the checksum */
	void Update(const Byte * a_Data, size_t a_Length);
	
	/** Calculates and returns the final checksum */
	void Finalize(Checksum & a_Output);
	
	/** Returns true if the object is accepts more input data, false if Finalize()-d (need to Restart()) */
	bool DoesAcceptInput(void) const { return m_DoesAcceptInput; }
	
	/** Converts a raw 160-bit SHA1 digest into a Java Hex representation
	According to http://wiki.vg/wiki/index.php?title=Protocol_Encryption&oldid=2802
	*/
	static void DigestToJava(const Checksum & a_Digest, AString & a_JavaOut);
	
	/** Clears the current context and start a new checksum calculation */
	void Restart(void);
	
protected:
	/** True if the object is accepts more input data, false if Finalize()-d (need to Restart()) */
	bool m_DoesAcceptInput;
	
	sha1_context m_Sha1;
} ;




