#include <strips_prob.hxx>
#include <action.hxx>
#include <fluent.hxx>
#include <cassert>
#include <map>

namespace aptk
{

	STRIPS_Problem::STRIPS_Problem()
		: m_num_fluents( 0 ), m_num_actions( 0 ), m_end_operator_id( no_such_index )
	{
	}

	STRIPS_Problem::~STRIPS_Problem()
	{
	}

	void	STRIPS_Problem::make_action_tables()
	{
		m_requiring.resize( fluents().size() );
		m_deleting.resize( fluents().size() );
		m_adding.resize( fluents().size() );
		
		for ( unsigned k = 0; k < actions().size(); k++ )
			register_action_in_tables( actions()[k] );
	}

	void	STRIPS_Problem::register_action_in_tables( Action* a )
	{
		for ( unsigned k = 0; k < a->prec_vec().size(); k++ )
			actions_requiring(a->prec_vec()[k]).push_back( a );
		for ( unsigned k = 0; k < a->add_vec().size(); k++ )
			actions_adding(a->add_vec()[k]).push_back(a);
		for ( unsigned k = 0; k < a->del_vec().size(); k++ )
			actions_deleting(a->del_vec()[k]).push_back(a);	
		
		//register conditional effects
		for ( unsigned i = 0; i < a->ceff_vec().size(); i++ )
		{
			for ( unsigned k = 0; k < a->ceff_vec()[i]->prec_vec().size(); k++ )
				actions_requiring(a->ceff_vec()[i]->prec_vec()[k]).push_back( a );
			for ( unsigned k = 0; k < a->ceff_vec()[i]->add_vec().size(); k++ )
				actions_adding(a->ceff_vec()[i]->add_vec()[k]).push_back(a);
			for ( unsigned k = 0; k < a->ceff_vec()[i]->del_vec().size(); k++ )
				actions_deleting(a->ceff_vec()[i]->del_vec()[k]).push_back(a);	
		}
	}
	
	unsigned STRIPS_Problem::add_action( STRIPS_Problem& p, std::string signature,
					     Fluent_Vec& pre, Fluent_Vec& add, Fluent_Vec& del,
					     Conditional_Effect_Vec& ceffs )
	{
		Action* new_act = new Action( p );
		new_act->set_signature( signature );
		new_act->define( pre, add, del, ceffs );
		p.increase_num_actions();
		p.actions().push_back( new_act );
		new_act->set_index( p.actions().size()-1 );
		return p.actions().size()-1;
	}

	unsigned STRIPS_Problem::add_fluent( STRIPS_Problem& p, std::string signature )
	{
		Fluent* new_fluent = new Fluent( p );
		new_fluent->set_index( p.fluents().size() );
		new_fluent->set_signature( signature );
		p.m_fluents_map[signature] = new_fluent->index();
		p.increase_num_fluents();
		p.fluents().push_back( new_fluent );
		return p.fluents().size()-1;
	}

	void	STRIPS_Problem::set_init( STRIPS_Problem& p, Fluent_Vec& init_vec )
	{
#ifdef DEBUG
		for ( unsigned k = 0; k < init_vec.size(); k++ )
			assert( init_vec[k] < p.num_fluents() );
#endif	
		if ( p.m_in_init.empty() )
			p.m_in_init.resize( p.num_fluents(), false );
		else
			for ( unsigned k = 0; k < p.num_fluents(); k++ )
				p.m_in_init[k] = false;

		p.init().assign( init_vec.begin(), init_vec.end() );

		for ( unsigned k = 0; k < init_vec.size(); k++ )
			p.m_in_init[ init_vec[k] ] = true;
	}

	void	STRIPS_Problem::set_goal( STRIPS_Problem& p, Fluent_Vec& goal_vec, bool create_end_op )
	{
#ifdef DEBUG
		for ( unsigned k = 0; k < goal_vec.size(); k++ )
			assert( goal_vec[k] < p.num_fluents() );
#endif
		if ( p.m_in_goal.empty() )
			p.m_in_goal.resize( p.num_fluents(), false );
		else
			for ( unsigned k = 0; k < p.num_fluents(); k++ )
				p.m_in_goal[k] = false;

		p.goal().assign( goal_vec.begin(), goal_vec.end() );

		for ( unsigned k = 0; k < goal_vec.size(); k++ )
			p.m_in_goal[ goal_vec[k] ] = true;
		
		if ( create_end_op )
		{
			Fluent_Vec dummy;
			Conditional_Effect_Vec dummy_ceffs;
			p.m_end_operator_id = add_action( p, "(END)", goal_vec, dummy, dummy, dummy_ceffs);
			p.actions()[ p.m_end_operator_id ]->set_cost( 0 );
		}
	}

	void STRIPS_Problem::print_fluent_vec(const Fluent_Vec &a) {
		for(unsigned i = 0; i < a.size(); i++) {
			std::cout << fluents()[a[i]]->signature() << ", ";
		}
	}

        unsigned STRIPS_Problem::get_fluent_index( std::string signature ) {
              return m_fluents_map[signature];
        }
}
