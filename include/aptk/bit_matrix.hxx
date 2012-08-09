#ifndef __BIT_SQUARE_MATRIX__
#define __BIT_SQUARE_MATRIX__

#include <bit_array.hxx>

namespace aptk
{

class Bit_Matrix
{
public:

	Bit_Matrix()
		: m_data(NULL), m_M(0), m_N( 0 )
	{
	}
	
	Bit_Matrix( unsigned M, unsigned N )
		: m_data(NULL), m_M(M), m_N( N )
	{
		resize( m_M*m_N );
	}

	Bit_Matrix( Bit_Matrix& other )
	{
		m_N = other.m_N;
		m_M = other.m_M;
		m_data = new Bit_Array( *(other.m_data) );
	}
	
	~Bit_Matrix()
	{
		if ( m_data ) delete m_data;
	}

	void set_dims( unsigned M, unsigned N )
	{
		m_N = N;
		m_M = M;
		resize( m_M*m_N );
	}
	
	void set( unsigned i, unsigned j )
	{
		m_data->set( i*m_N+j );
	}
	
	void unset( unsigned i, unsigned j )
	{
		m_data->unset( i*m_N+j);
	}

	unsigned isset( unsigned i, unsigned j )
	{
		return m_data->isset( i*m_N+j );
	}

	void clear() 
	{
		m_data->reset();
	}	

	void resize( unsigned sz )
	{
		m_data = new Bit_Array(sz);
		m_data->reset();
	}
	
protected:
	Bit_Array*   m_data;
	unsigned     m_M;
	unsigned     m_N;
};

}

#endif // square_matrix.hxx
