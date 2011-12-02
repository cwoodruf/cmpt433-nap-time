<?php
/**
 * in this case we used a different default controller for the home page
 * in general its probably confusing to do this esp as the template for the
 * home page has a different name than the controller: ie DON'T DO THIS :)
 *
 * this is a demo / test of the paging tools - these may need to be further refined
 */
class Index extends Controller {
	const PAGESIZE = 11;

	public function execute() {

		# use the remembered state to get the correct page
		if (!isset($_REQUEST['offset'])) 
			$offset = Entity::getpageid('numbered','offset');
		else 
			$offset = $_REQUEST['offset'];

		# how to get a page of records from the db
		$page = Entity::getpage(
			'numbered',
			(new Numbered),
			$offset,
			self::PAGESIZE,
			'order by created desc',
			array('*',"length(notes) as characters")
		);

		# these are needed by the pagerlinks plugin 
		View::assign('limit',$page['limit']);
		View::assign('howmany',$page['howmany']);
		View::assign('offset',$page['offset']);
		View::assign('notes',$page['rows']);

		View::display('home.tpl');
	}
}

