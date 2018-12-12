{
	double fgDr = 5.3570988356489;
	double fgRmin = 1440.;
	double fgGap = 0.10;
	double fgDphi[] = { 0.00082390388954424, 0.00075576971366224};
	int    fgNpads[] = { 176, 192 };
	for (int l=0; l<28; l++) {
		int section = l < 14 ? 0 : 1;
		double r = fgRmin + fgDr*(l+0.5) - 0.5*fgGap;
		double pitch = r*fgDphi[section];
		double width = pitch - fgGap;
		double offset = l%2 ? pitch/2 : 0.;
		cout << '\t' << "    " 
		     << "<!-- row " << l << " -->" << endl;
		cout << '\t' << "    "
		     << "<row nPad=\"" << fgNpads[section]  << "\" "
		     << "padPitch=\"" << pitch << "\" "
		     << "rowHeight=\"" << fgDr << "\" " << endl;
		cout << '\t' << "         "
		     << "padWidth=\"" << width << "\" "
		     << "padHeight=\"" << fgDr - fgGap << "\" ";
		if (l%2) {
			cout << "offset=\"" << offset << "\" ";
		}
		cout << "/>" << endl;
	}
}
