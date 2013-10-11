<?php
/**
 * Style plugin to render each item in a grouped list.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_rlist extends views_plugin_style_default {
	function render_grouping($records, $groupings = array(), $group_rendered = NULL) {
		// This is for backward compability, when $groupings was a string containing
		// the ID of a single field.
		if (is_string($groupings)) {
			$rendered = $group_rendered === NULL ? TRUE : $group_rendered;
			$groupings = array(array('field' => $groupings, 'rendered' => $rendered));
		}
	
		// Make sure fields are rendered
		$this->render_fields($this->view->result);
		$sets = array();
		if ($groupings) {
			foreach ($records as $index => $row) {
				// Iterate through configured grouping fields to determine the
				// hierarchically positioned set where the current row belongs to.
				// While iterating, parent groups, that do not exist yet, are added.
				$set = &$sets;
				foreach ($groupings as $info) {
					$field = $info['field'];
					$rendered = isset($info['rendered']) ? $info['rendered'] : $group_rendered;
					$rendered_strip = isset($info['rendered_strip']) ? $info['rendered_strip'] : FALSE;
					$grouping = '';
					$group_content = '';
					// Group on the rendered version of the field, not the raw.  That way,
					// we can control any special formatting of the grouping field through
					// the admin or theme layer or anywhere else we'd like.
					if (isset($this->view->field[$field])) {
						$group_content = $this->get_field($index, $field);
						if ($this->view->field[$field]->options['label']) {
							$group_content = $this->view->field[$field]->options['label'] . ': ' . $group_content;
						}
						if ($rendered) {
							$grouping = $group_content;
							if ($rendered_strip) {
								$group_content = $grouping = strip_tags(htmlspecialchars_decode($group_content));
							}
						}
						else {
							$grouping = $this->get_field_value($index, $field);
							// Not all field handlers return a scalar value,
							// e.g. views_handler_field_field.
							if (!is_scalar($grouping)) {
								$grouping = md5(serialize($grouping));
							}
						}
					}
					
					//
					$flats = explode(', ', $grouping);
					foreach ($flats as $flat) {	
						// Create the group if it does not exist yet.
						if (empty($set[$flat])) {
							$set[$flat]['group'] = $flat;
							$set[$flat]['rows'] = array();
						}
						
						// Add the row to the hierarchically positioned row set we just determined.
						$set[$flat]['rows'][$index] = $row;
						
						// Move the set reference into the row set of the group we just determined.
						//$set = &$set[$grouping]['rows'];
					}
				}
			}
		}
		else {
			// Create a single group with an empty grouping field.
			$sets[''] = array(
					'group' => '',
					'rows' => $records,
			);
		}
	
		// If this parameter isn't explicitely set modify the output to be fully
		// backward compatible to code before Views 7.x-3.0-rc2.
		// @TODO Remove this as soon as possible e.g. October 2020
		if ($group_rendered === NULL) {
			$old_style_sets = array();
			foreach ($sets as $group) {
				$old_style_sets[$group['group']] = $group['rows'];
			}
			$sets = $old_style_sets;
		}

		// custom sort
		$groups = array('Head of the Institute', 'Professors', 'Senior Research Associates', 'Research Assistants', 'Technical Specialists', 'Secretary');
		$ordered_sets = array();
		foreach ($groups as $group) {
			if (isset($sets[$group])) $ordered_sets[$group] = $sets[$group];
		}

		// RAs & PhDs merge
		if (isset($sets['Ph.D. Students']['rows'])) {
			foreach ($sets['Ph.D. Students']['rows'] as $i => $item) {
				$ordered_sets['Research Assistants']['rows'][$i] = $item;
			}
			ksort($ordered_sets['Research Assistants']['rows']);
			$ordered_sets['Research Assistants']['group'] .= ' and Ph.D. Students';
		}
		// oh my: dont know how to exclude Teaching group right now:
		// should be done in views filter 
		if (isset($sets['<a href="/tag/teaching">Teaching</a>'])) { unset($sets['<a href="/tag/teaching">Teaching</a>']); }
		
		$return = empty($ordered_sets) ? $sets : $ordered_sets;
		return $return;
	}
}
