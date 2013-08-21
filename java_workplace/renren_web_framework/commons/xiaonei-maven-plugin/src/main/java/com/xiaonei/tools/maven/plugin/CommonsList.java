package com.xiaonei.tools.maven.plugin;

import java.util.HashSet;
import java.util.Set;

import org.apache.maven.artifact.Artifact;
import org.apache.maven.artifact.metadata.ArtifactMetadataSource;
import org.apache.maven.artifact.resolver.ArtifactNotFoundException;
import org.apache.maven.artifact.resolver.ArtifactResolutionException;
import org.apache.maven.artifact.resolver.ArtifactResolutionResult;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;
import org.apache.maven.project.MavenProjectBuilder;
import org.apache.maven.project.ProjectBuildingException;
import org.apache.maven.project.artifact.InvalidDependencyVersionException;
import org.apache.maven.shared.artifact.filter.collection.ArtifactFilterException;
import org.apache.maven.shared.artifact.filter.collection.FilterArtifacts;
import org.apache.maven.shared.artifact.filter.collection.GroupIdFilter;

/**
 * 
 * @goal list
 * 
 */
public class CommonsList extends AbstractMojo{
	/**
	 * Used to look up Artifacts in the remote repository.
	 * 
	 * @component
	 */
	protected org.apache.maven.artifact.factory.ArtifactFactory factory;
	
	/** @component */
	private MavenProjectBuilder mavenProjectBuilder;
	
	/**
	 * List of Remote Repositories used by the resolver
	 * 
	 * @parameter expression="${project.remoteArtifactRepositories}"
	 * @required
	 */
	protected java.util.List remoteRepos;
	
	/**
	 * Location of the local repository.
	 * 
	 * @parameter expression="${localRepository}"
	 * @readonly
	 * @required
	 */
	protected org.apache.maven.artifact.repository.ArtifactRepository localRepos;
	
	/**
	 * Specify classifier to look for. Example: sources
	 * 
	 * @optional
	 * @since 2.0
	 * @parameter expression="${classifier}" default-value=""
	 */
	protected String classifier;
	
	/**
	 * POM
	 * 
	 * @parameter expression="${project}"
	 * @required
	 */
	protected MavenProject	project;
	
	/**
	 * Used to look up Artifacts in the remote repository.
	 * 
	 * @component
	 */
	protected org.apache.maven.artifact.resolver.ArtifactResolver resolver;
	
	/** @component */
	private ArtifactMetadataSource artifactMetadataSource;
	
	/**
	 * @parameter default-value=true
	 */
	private boolean force;

	/**
	 * Comma separated list of GroupId Names to exclude.
	 * 
	 * @since 2.0
	 * @optional
	 * @parameter expression="${excludeGroupIds}" default-value="com.xiaonei,com.kaixin"
	 */
	protected String excludeGroupIds;
	
	@Override
	public void execute() throws MojoExecutionException, MojoFailureException {
		Set commons = getCommonsDependencies();
		Set artifacts = getCurrentDependencies();
		FilterArtifacts filter = new FilterArtifacts();
		filter.addFilter(new GroupIdFilter("",excludeGroupIds));
		
		try {
			artifacts = filter.filter(artifacts);
		} catch (ArtifactFilterException e) {
			e.printStackTrace();
		}
		
		StringBuffer buffer = new StringBuffer();
		buffer.append("\n");
		buffer.append("The maven plugin develop by web framework team\n");
		buffer.append("The following files is not in commons version:\n");
		for(Object obj:artifacts){
			Artifact artifact = (Artifact)obj;
			if(!commons.contains(obj)){
				buffer.append("    "+artifact+"\n");
			}
		}
		
		if(buffer.length() > 0){
			getLog().info(buffer);
			if(force)throw new MojoExecutionException("");
		}
	}
	
	private Set getCommonsDependencies(){
		Artifact artifact = factory.createArtifact("com.xiaonei", "xiaonei-3rd-commons", "1.0-SNAPSHOT", classifier, "pom");
		MavenProject commonsProject = null;
		try {
			commonsProject = mavenProjectBuilder.buildFromRepository(artifact, remoteRepos, localRepos);
		} catch (ProjectBuildingException e) {
			e.printStackTrace();
		}
		
		Set<Artifact> artifacts = null;
		try {
			artifacts = commonsProject.createArtifacts(factory, null, null);
		} catch (InvalidDependencyVersionException e) {
			e.printStackTrace();
		}
		
		return artifacts;
	}
	
	private Set getCurrentDependencies(){
		Set<Artifact> artifacts=null;
		try {
			artifacts = project.createArtifacts(factory, null, null);
		} catch (InvalidDependencyVersionException e) {
			e.printStackTrace();
		}
		
		Set<Artifact> resolvedArtifacts = new HashSet<Artifact>();
	
		try {
			ArtifactResolutionResult result = resolver.resolveTransitively(artifacts, project.getArtifact(),remoteRepos, localRepos, artifactMetadataSource);
			artifacts = result.getArtifacts();
			resolvedArtifacts.addAll(artifacts);
		} catch (ArtifactResolutionException e) {
			e.printStackTrace();
		} catch (ArtifactNotFoundException e) {
			e.printStackTrace();
		}
		
		return resolvedArtifacts;
	}
}
